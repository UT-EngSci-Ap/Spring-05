#include "Screen.h"

Screen::Screen(UIRenderer& ui, InputHandler& input)
    : ui_(ui), input_(input) {}

ScreenId Screen::run() {
    while (true) {
        render();
        ScreenId next = handleInput();
        if (next != ScreenId::None) {
            return next;
        }
    }
}
