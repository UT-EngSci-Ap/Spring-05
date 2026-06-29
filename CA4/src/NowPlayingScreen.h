#pragma once

#include <functional>

#include "Screen.h"

class Player;
class Favourites;

class NowPlayingScreen : public Screen {
public:
    NowPlayingScreen(UIRenderer& ui, InputHandler& input, Player& player,
                     Favourites& favourites,
                     std::function<void()> onFavouritesChanged);

    // Live, raw-input loop: redraws the elapsed/total timer every refresh tick
    // without waiting for a keypress, auto-advances when a song ends, and reacts
    // to single keypresses (space play/pause, n/b, ←/→ seek, f favourite, q).
    ScreenId run() override;

    void render() override;
    ScreenId handleInput() override;

private:
    ScreenId dispatchKey(int keyCode);

    Player& player_;
    Favourites& favourites_;
    std::function<void()> onFavouritesChanged_;
};
