#pragma once

#include <string>

enum class Color {
    Default, Cyan, Green, Yellow, Gray, Red, Magenta, White, Dim
};

// Centralises ALL terminal output. Lower layers (Song, Playlist, Player...)
// never call std::cout directly; they go through this class via the screens.
class UIRenderer {
public:
    static constexpr int kWidth = 56;   // inner width between box borders

    void clearScreen() const;

    // Cursor control for flicker-free live screens (e.g. Now Playing): move the
    // cursor home and redraw a fixed-height frame in place instead of clearing.
    void home() const;          // move cursor to top-left, no clear
    void hideCursor() const;
    void showCursor() const;
    void flush() const;         // force buffered output to the terminal

    void boxTop() const;
    void boxSep() const;
    void boxBottom() const;
    void boxLine(const std::string& text, Color color = Color::Default) const;
    void boxCentered(const std::string& text, Color color = Color::White) const;

    void println(const std::string& text = "",
                 Color color = Color::Default) const;
    void print(const std::string& text, Color color = Color::Default) const;

private:
    std::string code(Color color) const;
    std::string colorize(const std::string& text, Color color) const;
    std::string fit(const std::string& text, int width) const;
    std::string center(const std::string& text, int width) const;
    std::string repeat(const std::string& unit, int times) const;
};
