#include "Game.h"
#include <SFML/Window.hpp>
#include <iostream>
#include "MainMenu.h"
#include "ResourceManager.h"

constexpr unsigned int WINDOW_WIDTH = 900;
constexpr unsigned int WINDOW_HEIGHT = 600;

Game::Game() : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Event Handling"),
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

Game::~Game()
{
    if (main_menu != nullptr)
    {
        delete main_menu;
    }
}
