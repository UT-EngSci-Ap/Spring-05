#pragma once

#include "Screen.h"

class Player;
class ConfigManager;

class SettingsScreen : public Screen {
public:
    SettingsScreen(UIRenderer& ui, InputHandler& input, Player& player,
                   ConfigManager& config);

    void render() override;
    ScreenId handleInput() override;

private:
    Player& player_;
    ConfigManager& config_;
};
