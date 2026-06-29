#pragma once

#include <termios.h>

#include <string>

// Special key codes returned by InputHandler::readKey. Printable keys are
// returned as their own ASCII value (e.g. 'p' == 112), so the special codes
// live well above the ASCII range to avoid any collision.
namespace key {
constexpr int None  = 0;        // no key arrived within the timeout
constexpr int Up    = 1000;
constexpr int Down  = 1001;
constexpr int Left  = 1002;
constexpr int Right = 1003;
constexpr int Esc   = 1004;
constexpr int Enter = '\n';
}  // namespace key

// Puts the terminal into raw (non-canonical, no-echo) mode for its lifetime
// and restores the previous settings on destruction (RAII). This is what makes
// single-keypress and arrow-key input possible without pressing Enter.
// A no-op when stdin is not a TTY, so piped input still works.
class RawMode {
public:
    RawMode();
    ~RawMode();

    RawMode(const RawMode&) = delete;
    RawMode& operator=(const RawMode&) = delete;

private:
    struct termios original_{};
    bool active_ = false;
};

// Centralises and validates ALL user input. Lower layers never read from
// std::cin directly. Line-based (std::getline) input is used for menus so a
// non-numeric entry can never put the stream into a fail state / infinite loop;
// readKey() adds raw single-key input for the live Now Playing screen.
class InputHandler {
public:
    // Reads a whole line, trimmed of leading/trailing whitespace.
    std::string readLine(const std::string& prompt) const;

    // Reads an integer in [min, max]; re-prompts on invalid input.
    int readInt(const std::string& prompt, int min, int max) const;

    // Waits up to timeoutMs for input to become available on stdin.
    // Returns true if input is ready, false if the timeout elapsed first.
    bool waitForInput(int timeoutMs) const;

    // Reads a single keypress, waiting up to timeoutMs. Returns one of the
    // key:: codes for special keys, the ASCII value for printable keys, or
    // key::None on timeout. Requires a RawMode to be active for special keys
    // (arrows) and to avoid blocking on Enter.
    int readKey(int timeoutMs) const;

    std::string trim(const std::string& s) const;
};
