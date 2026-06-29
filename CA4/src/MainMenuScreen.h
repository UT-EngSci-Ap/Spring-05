#pragma once

#include "Screen.h"

class Player;

class MainMenuScreen : public Screen {
public:
    MainMenuScreen(UIRenderer& ui, InputHandler& input, Player& player);

    void render() override;
    ScreenId handleInput() override;

private:
    Player& player_;
};
