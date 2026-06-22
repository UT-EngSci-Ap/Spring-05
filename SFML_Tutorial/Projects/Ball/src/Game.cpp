#include "Game.h"
#include <SFML/Window.hpp>
#include <iostream>
#include "MainMenu.h"
#include "ResourceManaegr.h"

Game::Game() : window(sf::VideoMode({900, 600}), "Event Handling"),
               resource_manager("../../img"),
               current_state(nullptr)
{
    current_state = new MainMenu(resource_manager, window.getSize());
}

void Game::run()
{

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            current_state->handleEvent(*event);
        }
        window.clear(sf::Color::Black);
        current_state->draw(window);
        window.display();
        if (current_state->getTransition() == EXIT)
        {
            window.close();
        }
    }
}