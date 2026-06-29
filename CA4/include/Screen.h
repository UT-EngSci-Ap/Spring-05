#pragma once

class UIRenderer;
class InputHandler;

// Identifies which screen the controller should show next.
// `None` means "stay on the current screen".
enum class ScreenId {
    None, MainMenu, NowPlaying, PlaylistList, PlaylistView, Settings, Quit
};

// Abstract base for every TUI screen. Demonstrates the required class
// hierarchy: a pure-virtual render() (abstraction) plus a virtual destructor,
// driven polymorphically through a Screen* in Application.
class Screen {
public:
    Screen(UIRenderer& ui, InputHandler& input);
    virtual ~Screen() = default;

    virtual void render() = 0;
    virtual ScreenId handleInput() = 0;

    // Render/input loop; returns the next screen to show.
    // virtual so a screen (e.g. Now Playing) can use a timed, non-blocking
    // loop instead of the default blocking one.
    virtual ScreenId run();

protected:
    UIRenderer& ui_;
    InputHandler& input_;
};
