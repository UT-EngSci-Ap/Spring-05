#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Screen.h"

class Player;
class ConfigManager;
class Playlist;

class PlaylistListScreen : public Screen {
public:
    PlaylistListScreen(UIRenderer& ui, InputHandler& input, Player& player,
                       ConfigManager& config,
                       std::vector<std::unique_ptr<Playlist>>& playlists,
                       std::function<void()> onRefresh);

    void render() override;
    ScreenId handleInput() override;

private:
    Player& player_;
    ConfigManager& config_;
    std::vector<std::unique_ptr<Playlist>>& playlists_;
    std::function<void()> onRefresh_;   // rebuilds auto playlists before listing
};
