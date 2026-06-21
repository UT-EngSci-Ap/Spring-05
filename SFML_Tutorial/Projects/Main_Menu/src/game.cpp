#include "game.h"
#include <SFML/Window.hpp>
#include <iostream>
#include "MainMenu.h"

void Game::processEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        mainMenu->handleEvent(*event);
    }
}

void Game::run()
{
    window.create(sf::VideoMode({900, 600}), "Event Handling");

    while (window.isOpen())
    {
        processEvents();
        window.clear(sf::Color::Black);
        mainMenu->draw(window);
        window.display();
    }
}