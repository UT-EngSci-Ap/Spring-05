#include "Game.h"
#include <SFML/Window.hpp>
#include <iostream>
#include "MainMenu.h"
#include "ResourceManaegr.h"

Game::Game() : window(sf::VideoMode({900, 600}), "Event Handling"),
               resource_manager("../../img"),
               mainMenu(nullptr)
{
    mainMenu = new MainMenu(resource_manager, window.getSize());
}

void Game::processEvents()
{
}

void Game::run()
{

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            mainMenu->handleEvent(*event);
        }
        window.clear(sf::Color::Black);
        mainMenu->draw(window);
        window.display();
        if (mainMenu->getTransition() == EXIT)
        {
            window.close();
        }
    }
}