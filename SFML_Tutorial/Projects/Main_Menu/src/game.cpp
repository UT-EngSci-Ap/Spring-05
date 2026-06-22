#include "Game.h"
#include <SFML/Window.hpp>
#include <iostream>
#include "MainMenu.h"
#include "ResourceManager.h"

Game::Game() : window(sf::VideoMode({900, 600}), "Event Handling"),
               resourceManager("../../img"),
               mainMenu(nullptr)
{
    mainMenu = new MainMenu(resourceManager, window.getSize());
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