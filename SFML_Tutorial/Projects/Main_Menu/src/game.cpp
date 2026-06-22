#include "Game.h"
#include <SFML/Window.hpp>
#include <iostream>
#include "MainMenu.h"
#include "ResourceManager.h"

Game::Game() : window(sf::VideoMode({900, 600}), "Event Handling"),
               resource_manager("../../img"),
               main_menu(nullptr)
{
    main_menu = new MainMenu(resource_manager, window.getSize());
}

void Game::run()
{

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            main_menu->handleEvent(*event);
        }
        window.clear(sf::Color::Black);
        main_menu->draw(window);
        window.display();
        if (main_menu->getTransition() == EXIT)
        {
            window.close();
        }
    }
}