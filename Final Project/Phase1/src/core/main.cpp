#include "core/Game.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>

int main() {

    srand(static_cast<unsigned int>(time(nullptr)));

    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
