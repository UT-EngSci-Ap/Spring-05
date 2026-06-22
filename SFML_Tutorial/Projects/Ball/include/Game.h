#ifndef __GAME__
#define __GAME__

#include <SFML/Window.hpp>
#include "MainMenu.h"
#include "ResourceManager.h"
#include "State.h"
class Game
{
private:
    sf::RenderWindow window;
    ResourceManager resource_manager;
    State *current_state;

public:
    Game();
    void run();
};

#endif