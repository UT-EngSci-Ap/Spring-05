#ifndef __GAME__
#define __GAME__

#include <SFML/Window.hpp>
#include "MainMenu.h"
#include "ResourceManager.h"

class Game
{
private:
    sf::RenderWindow window;
    ResourceManager resource_manager;
    MainMenu *main_menu;
    void processEvents();

public:
    ~Game();
    Game();
    void run();
};

#endif