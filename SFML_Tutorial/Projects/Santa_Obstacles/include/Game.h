#ifndef __GAME__
#define __GAME__

#include <SFML/Window.hpp>
#include "MainMenu.h"
#include "ResourceManager.h"
#include "State.h"
class Game
{
private:
    sf::Clock clock;
    sf::RenderWindow window;
    ResourceManager resource_manager;
    State *current_state;

public:
    Game();
    ~Game();
    void run();
    void handleTransition();
};

#endif