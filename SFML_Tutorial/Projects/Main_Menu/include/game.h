#ifndef __GAME__
#define __GAME__

#include <SFML/Window.hpp>
#include "MainMenu.h"
#include "ResourceManaegr.h"

class Game
{
private:
    sf::RenderWindow window;
    ResourceManager resource_manager;
    MainMenu *mainMenu;
    void processEvents();

public:
    Game();
    void run();
};

#endif