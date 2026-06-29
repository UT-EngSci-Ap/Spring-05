#include "PlaylistListScreen.h"

#include <iomanip>
#include <sstream>

#include <utility>

#include "ConfigManager.h"
#include "InputHandler.h"
#include "Player.h"
#include "Playlist.h"
#include "UIRenderer.h"

PlaylistListScreen::PlaylistListScreen(
    UIRenderer& ui, InputHandler& input, Player& player,
    ConfigManager& config,
    std::vector<std::unique_ptr<Playlist>>& playlists,
    std::function<void()> onRefresh)
    : Screen(ui, input),
      player_(player),
      config_(config),
      playlists_(playlists),
      onRefresh_(std::move(onRefresh)) {}

void PlaylistListScreen::render() {
    if (onRefresh_) {
        onRefresh_();   // ♥ Favourites / ★ Most Played reflect current state
    }
    ui_.clearScreen();
    ui_.boxTop();
    ui_.boxCentered("Playlists", Color::White);
    ui_.boxSep();
    ui_.boxLine("  #   Name                          Songs", Color::White);
    ui_.boxSep();

    if (playlists_.empty()) {
        ui_.boxLine("  (no playlists found)", Color::Gray);
    } else {
        Playlist* active = player_.playlist();
        for (std::size_t i = 0; i < playlists_.size(); ++i) {
            Playlist* pl = playlists_[i].get();
            bool isActive = (pl == active);

            std::ostringstream row;
            row << "  " << std::left << std::setw(4) << (i + 1)
                << std::setw(28) << pl->name()
                << (isActive ? " ▶ " : "   ")
                << pl->size();
            if (isActive) {
                row << "  [active]";
            }
            ui_.boxLine(row.str(), isActive ? Color::Yellow : Color::Gray);
        }
    }

    ui_.boxSep();
    ui_.boxLine("  Enter number to switch active playlist.  [0] back",
                Color::Dim);
    ui_.boxBottom();
}

ScreenId PlaylistListScreen::handleInput() {
    int max = static_cast<int>(playlists_.size());
    int choice = input_.readInt("Choice: ", 0, max);
    if (choice == 0) {
        return ScreenId::MainMenu;
    }

    Playlist* chosen = playlists_[static_cast<std::size_t>(choice - 1)].get();
    if (chosen != player_.playlist()) {
        player_.setPlaylist(chosen);   // stops current playback
        config_.set("active_playlist", chosen->name());
        config_.save();
    }
    return ScreenId::None;
}
