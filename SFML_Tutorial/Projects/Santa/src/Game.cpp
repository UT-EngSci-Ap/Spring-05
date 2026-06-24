#include "Game.h"
#include <SFML/Window.hpp>
#include <iostream>
#include "MainMenu.h"
#include "Play.h"
#include "ResourceManager.h"

Game::Game() : window(sf::VideoMode({900, 600}), "Event Handling"),
               resource_manager("../../assets/img", "../../assets/font/Roboto/static"),
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
        current_state->update(clock.restart().asMilliseconds());
        window.clear(sf::Color::Black);
        current_state->draw(window);
        window.display();
        handleTransition();
    }
}

void Game::handleTransition()
{
    switch (current_state->getTransition())
    {
    case PLAY:
    {
        delete current_state;
        current_state = new Play(resource_manager, window.getSize());
        break;
    }
    case EXIT:
    {
        delete current_state;
        window.close();
        break;
    }
    }
}

Game::~Game()
{
    if (current_state != nullptr)
    {
        delete current_state;
    }
}
