#pragma once

#include <memory>
#include <vector>

#include "ConfigManager.h"
#include "InputHandler.h"
#include "MusicLibrary.h"
#include "Player.h"
#include "UIRenderer.h"

class Playlist;
class Screen;
class PlayHistory;
class Favourites;
enum class ScreenId;

// Top-level controller. Owns every subsystem, wires the screens together and
// runs the main navigation loop. Uses unique_ptr for leak-free ownership of
// playlists and screens; Song objects are owned solely by MusicLibrary.
class Application {
public:
    Application();
    ~Application();

    bool init();      // load data, build subsystems and screens
    void run();       // main screen loop
    void shutdown();  // persist state to settings.cfg

private:
    Screen* screenFor(ScreenId id) const;
    Playlist* findPlaylist(const std::string& name) const;

    // Regenerates the auto playlists (♥ Favourites, ★ Most Played) from the
    // current favourites set and play counts. Called after any change.
    void refreshAutoPlaylists();

    // Fixed data paths, relative to the project root.
    static constexpr const char* kCsvPath = "Data/library.csv";
    static constexpr const char* kPlaylistDir = "Data/Playlists";
    static constexpr const char* kConfigPath = "Data/settings.cfg";
    static constexpr const char* kHistoryPath = "Data/play_history.csv";
    static constexpr const char* kFavouritesPath = "Data/favourites.m3u";
    static constexpr int kMostPlayedLimit = 10;

    MusicLibrary library_;
    std::vector<std::unique_ptr<Playlist>> playlists_;
    UIRenderer ui_;
    InputHandler input_;
    std::unique_ptr<ConfigManager> config_;
    std::unique_ptr<PlayHistory> history_;
    std::unique_ptr<Favourites> favourites_;
    std::unique_ptr<Player> player_;

    // Auto-generated playlists owned within playlists_; cached here so they can
    // be rebuilt in place. Non-owning pointers into the playlists_ vector.
    Playlist* favPlaylist_ = nullptr;
    Playlist* mostPlayedPlaylist_ = nullptr;

    std::unique_ptr<Screen> mainMenu_;
    std::unique_ptr<Screen> nowPlaying_;
    std::unique_ptr<Screen> playlistList_;
    std::unique_ptr<Screen> playlistView_;
    std::unique_ptr<Screen> settings_;
};
