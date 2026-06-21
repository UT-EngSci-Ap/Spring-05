#ifndef __GAME__
#define __GAME__

#include <SFML/Window.hpp>
#include "MainMenu.h"

class Game
{
private:
    sf::RenderWindow window;
    std::pair<int, int> mouse_position;
    ResourceManager resourceManager;
    MainMenu *mainMenu;
    void processEvents();

public:
    Game() : mouse_position({0, 0}), resourceManager("../../img"), mainMenu(nullptr)
    {
        mainMenu = new MainMenu(resourceManager);
    }
    void run();
};

#endif