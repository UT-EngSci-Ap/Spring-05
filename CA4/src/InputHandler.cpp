#include "InputHandler.h"

#include <poll.h>
#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <string>

namespace {
constexpr int kEscByte = 27;        // ESC, also the lead byte of arrow keys
constexpr int kEscFollowupMs = 20;  // grace period to read the rest of "ESC[A"
}  // namespace

RawMode::RawMode() {
    if (!isatty(STDIN_FILENO)) {
        return;   // piped/redirected input: nothing to switch, leave inactive
    }
    if (tcgetattr(STDIN_FILENO, &original_) != 0) {
        return;
    }
    struct termios raw = original_;
    raw.c_lflag &= ~(ICANON | ECHO);   // char-at-a-time, no echo
    raw.c_cc[VMIN] = 0;                 // non-blocking read: poll decides timing
    raw.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == 0) {
        active_ = true;
    }
}

RawMode::~RawMode() {
    if (active_) {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_);
    }
}

std::string InputHandler::trim(const std::string& s) const {
    const char* ws = " \t\r\n";
    std::size_t begin = s.find_first_not_of(ws);
    if (begin == std::string::npos) {
        return "";
    }
    std::size_t end = s.find_last_not_of(ws);
    return s.substr(begin, end - begin + 1);
}

std::string InputHandler::readLine(const std::string& prompt) const {
    std::cout << prompt << std::flush;
    std::string line;
    if (!std::getline(std::cin, line)) {
        // EOF / stream closed: clear so the program can exit cleanly.
        std::cin.clear();
        return "";
    }
    return trim(line);
}

int InputHandler::readInt(const std::string& prompt, int min, int max) const {
    while (true) {
        std::string line = readLine(prompt);
        try {
            std::size_t pos = 0;
            int value = std::stoi(line, &pos);
            if (pos == line.size() && value >= min && value <= max) {
                return value;
            }
        } catch (const std::exception&) {
            // fall through to the error message
        }
        std::cout << "Invalid choice. Please try again.\n";
    }
}

bool InputHandler::waitForInput(int timeoutMs) const {
    struct pollfd pfd;
    pfd.fd = STDIN_FILENO;
    pfd.events = POLLIN;
    int ready = poll(&pfd, 1, timeoutMs);
    return ready > 0 && (pfd.revents & POLLIN) != 0;
}

int InputHandler::readKey(int timeoutMs) const {
    if (!waitForInput(timeoutMs)) {
        return key::None;
    }
    unsigned char c = 0;
    if (read(STDIN_FILENO, &c, 1) != 1) {
        return key::None;
    }
    if (c != kEscByte) {
        return static_cast<int>(c);
    }

    // Possible escape sequence: ESC '[' (or 'O') final-byte, e.g. arrow keys.
    // If nothing follows quickly it was a bare ESC press.
    if (!waitForInput(kEscFollowupMs)) {
        return key::Esc;
    }
    unsigned char bracket = 0;
    if (read(STDIN_FILENO, &bracket, 1) != 1 ||
        (bracket != '[' && bracket != 'O')) {
        return key::Esc;
    }
    if (!waitForInput(kEscFollowupMs)) {
        return key::Esc;
    }
    unsigned char code = 0;
    if (read(STDIN_FILENO, &code, 1) != 1) {
        return key::Esc;
    }
    switch (code) {
        case 'A': return key::Up;
        case 'B': return key::Down;
        case 'C': return key::Right;
        case 'D': return key::Left;
        default:  return key::Esc;
    }
}
