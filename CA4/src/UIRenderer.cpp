#include "UIRenderer.h"

#include <cstdlib>
#include <iostream>

namespace {

// Counts visible characters (UTF-8 code points), ignoring continuation bytes.
int displayWidth(const std::string& s) {
    int n = 0;
    for (unsigned char c : s) {
        if ((c & 0xC0) != 0x80) {
            ++n;
        }
    }
    return n;
}

// Length in bytes of the UTF-8 sequence starting at the given lead byte.
std::size_t seqLen(unsigned char c) {
    if (c >= 0xF0) return 4;
    if (c >= 0xE0) return 3;
    if (c >= 0xC0) return 2;
    return 1;
}

const char* kReset = "\033[0m";

}  // namespace

void UIRenderer::clearScreen() const {
    // Flush any buffered output (e.g. the trailing "Command: " prompt, which
    // has no newline) BEFORE clearing. Otherwise system("clear") — a separate
    // process writing straight to the terminal — runs first and the leftover
    // buffered text resurfaces on top of the freshly cleared screen.
    std::cout.flush();
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void UIRenderer::home() const { std::cout << "\033[H"; }
void UIRenderer::hideCursor() const { std::cout << "\033[?25l"; }
void UIRenderer::showCursor() const { std::cout << "\033[?25h"; }
void UIRenderer::flush() const { std::cout.flush(); }

std::string UIRenderer::code(Color color) const {
    switch (color) {
        case Color::Cyan:    return "\033[36m";
        case Color::Green:   return "\033[32m";
        case Color::Yellow:  return "\033[33m";
        case Color::Gray:    return "\033[90m";
        case Color::Red:     return "\033[31m";
        case Color::Magenta: return "\033[35m";
        case Color::White:   return "\033[97m";
        case Color::Dim:     return "\033[2m";
        case Color::Default: return "";
    }
    return "";
}

std::string UIRenderer::colorize(const std::string& text, Color color) const {
    std::string c = code(color);
    if (c.empty()) {
        return text;
    }
    return c + text + kReset;
}

std::string UIRenderer::repeat(const std::string& unit, int times) const {
    std::string out;
    out.reserve(unit.size() * static_cast<std::size_t>(times));
    for (int i = 0; i < times; ++i) {
        out += unit;
    }
    return out;
}

// Pads (or truncates with an ellipsis) so the result is exactly `width`
// visible columns wide.
std::string UIRenderer::fit(const std::string& text, int width) const {
    int w = displayWidth(text);
    if (w <= width) {
        return text + std::string(static_cast<std::size_t>(width - w), ' ');
    }
    std::string out;
    int count = 0;
    std::size_t i = 0;
    while (i < text.size() && count < width - 1) {
        std::size_t len = seqLen(static_cast<unsigned char>(text[i]));
        out += text.substr(i, len);
        i += len;
        ++count;
    }
    out += "\xE2\x80\xA6";   // … (1 visible column)
    return out;
}

std::string UIRenderer::center(const std::string& text, int width) const {
    int w = displayWidth(text);
    if (w >= width) {
        return fit(text, width);
    }
    int left = (width - w) / 2;
    int right = width - w - left;
    return std::string(static_cast<std::size_t>(left), ' ') + text +
           std::string(static_cast<std::size_t>(right), ' ');
}

void UIRenderer::boxTop() const {
    std::cout << colorize("╔" + repeat("═", kWidth) + "╗",
                          Color::Cyan)
              << '\n';
}

void UIRenderer::boxSep() const {
    std::cout << colorize("╠" + repeat("═", kWidth) + "╣",
                          Color::Cyan)
              << '\n';
}

void UIRenderer::boxBottom() const {
    std::cout << colorize("╚" + repeat("═", kWidth) + "╝",
                          Color::Cyan)
              << '\n';
}

void UIRenderer::boxLine(const std::string& text, Color color) const {
    std::cout << colorize("║", Color::Cyan)
              << colorize(fit(text, kWidth), color)
              << colorize("║", Color::Cyan) << '\n';
}

void UIRenderer::boxCentered(const std::string& text, Color color) const {
    std::cout << colorize("║", Color::Cyan)
              << colorize(center(text, kWidth), color)
              << colorize("║", Color::Cyan) << '\n';
}

void UIRenderer::println(const std::string& text, Color color) const {
    std::cout << colorize(text, color) << '\n';
}

void UIRenderer::print(const std::string& text, Color color) const {
    std::cout << colorize(text, color);
}
