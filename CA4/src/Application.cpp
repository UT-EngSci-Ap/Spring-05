#include "Application.h"

#include <iostream>

#include "CsvLoader.h"
#include "Favourites.h"
#include "M3uLoader.h"
#include "MainMenuScreen.h"
#include "NowPlayingScreen.h"
#include "PlayHistory.h"
#include "Playlist.h"
#include "PlaylistListScreen.h"
#include "PlaylistViewScreen.h"
#include "Screen.h"
#include "SettingsScreen.h"
#include "Song.h"

Application::Application() = default;
Application::~Application() = default;

bool Application::init() {
    CsvLoader csvLoader;
    int songs = csvLoader.load(kCsvPath, library_);
    if (songs == 0) {
        std::cerr << "No songs loaded from " << kCsvPath
                  << ". Make sure you run the program from the project root.\n";
        return false;
    }

    M3uLoader m3uLoader;
    playlists_ = m3uLoader.loadAll(kPlaylistDir, library_);

    config_ = std::make_unique<ConfigManager>(kConfigPath);
    config_->load();

    player_ = std::make_unique<Player>();

    // Bonus subsystems: play-count history and the favourites (♥) set.
    history_ = std::make_unique<PlayHistory>(kHistoryPath);
    history_->load();
    favourites_ = std::make_unique<Favourites>(kFavouritesPath);
    favourites_->load();
    player_->setHistory(history_.get());

    // Auto-generated playlists, appended AFTER the user's loaded playlists so
    // the default active playlist (the first one) stays a real, user list.
    auto mostPlayed = std::make_unique<Playlist>("★ Most Played");
    mostPlayedPlaylist_ = mostPlayed.get();
    playlists_.push_back(std::move(mostPlayed));
    auto favourites = std::make_unique<Playlist>("♥ Favourites");
    favPlaylist_ = favourites.get();
    playlists_.push_back(std::move(favourites));
    refreshAutoPlaylists();

    // Restore playback mode.
    PlaybackMode mode;
    if (parseMode(config_->get("playback_mode"), mode)) {
        player_->setMode(mode);
    }

    // Restore active playlist (fall back to the first available).
    Playlist* active = findPlaylist(config_->get("active_playlist"));
    if (active == nullptr && !playlists_.empty()) {
        active = playlists_.front().get();
    }
    player_->setPlaylist(active);

    // Restore last played song for the Now Playing display (no auto-play).
    std::string lastSong = config_->get("last_song");
    if (!lastSong.empty()) {
        Song* song = library_.findByPath(lastSong);
        if (song != nullptr) {
            player_->setCurrentSong(song);
        }
    }

    // Build the screens.
    mainMenu_ = std::make_unique<MainMenuScreen>(ui_, input_, *player_);
    nowPlaying_ = std::make_unique<NowPlayingScreen>(
        ui_, input_, *player_, *favourites_,
        [this]() {                       // ♥ toggled: persist + rebuild list
            favourites_->save();
            refreshAutoPlaylists();
        });
    playlistList_ = std::make_unique<PlaylistListScreen>(
        ui_, input_, *player_, *config_, playlists_,
        [this]() { refreshAutoPlaylists(); });   // refresh counts before listing
    playlistView_ = std::make_unique<PlaylistViewScreen>(ui_, input_, *player_);
    settings_ =
        std::make_unique<SettingsScreen>(ui_, input_, *player_, *config_);

    return true;
}

Playlist* Application::findPlaylist(const std::string& name) const {
    if (name.empty()) {
        return nullptr;
    }
    for (const auto& pl : playlists_) {
        if (pl->name() == name) {
            return pl.get();
        }
    }
    return nullptr;
}

void Application::refreshAutoPlaylists() {
    if (favPlaylist_ != nullptr) {
        favPlaylist_->clear();
        for (Song* song : favourites_->resolve(library_)) {
            favPlaylist_->addSong(song);
        }
    }
    if (mostPlayedPlaylist_ != nullptr) {
        mostPlayedPlaylist_->clear();
        for (Song* song : history_->mostPlayed(
                 library_, static_cast<std::size_t>(kMostPlayedLimit))) {
            mostPlayedPlaylist_->addSong(song);
        }
    }
    if (player_ != nullptr) {
        player_->resyncIndex();   // active list contents may have changed
    }
}

Screen* Application::screenFor(ScreenId id) const {
    switch (id) {
        case ScreenId::MainMenu:     return mainMenu_.get();
        case ScreenId::NowPlaying:   return nowPlaying_.get();
        case ScreenId::PlaylistList: return playlistList_.get();
        case ScreenId::PlaylistView: return playlistView_.get();
        case ScreenId::Settings:     return settings_.get();
        default:                     return nullptr;
    }
}

void Application::run() {
    ScreenId current = ScreenId::MainMenu;
    while (current != ScreenId::Quit) {
        Screen* screen = screenFor(current);
        if (screen == nullptr) {
            break;
        }
        current = screen->run();
    }
}

void Application::shutdown() {
    if (player_->currentSong() != nullptr) {
        config_->set("last_song", player_->currentSong()->filePath());
    }
    if (player_->playlist() != nullptr) {
        config_->set("active_playlist", player_->playlist()->name());
    }
    config_->set("playback_mode", toString(player_->mode()));
    config_->save();

    history_->save();        // persist play counts and favourites on exit
    favourites_->save();

    ui_.clearScreen();
    ui_.println("State saved. Goodbye!", Color::Cyan);
}
