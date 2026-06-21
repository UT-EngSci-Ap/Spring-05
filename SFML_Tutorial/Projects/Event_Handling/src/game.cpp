#include "game.h"
#include <SFML/Window.hpp>
#include <iostream>

void Game::processEvents()
{
    window.handleEvents(
        [&](const sf::Event::Closed &)
        {
            onClosed();
        },
        [&](const sf::Event::MouseMoved &e)
        {
            onMouseMoved(e);
        },
        [&](const sf::Event::MouseButtonPressed &e)
        {
            onMouseButtonPressed(e);
        });
}

void Game::onMouseButtonPressed(const sf::Event::MouseButtonPressed &event)
{
    std::cout << "mouse position is: (" << mouse_position.first << ", " << mouse_position.second << ")\n";
}

void Game::onClosed()
{
    window.close();
}

void Game::onMouseMoved(const sf::Event::MouseMoved &event)
{
    mouse_position = {event.position.x, event.position.y};
}

void Game::run()
{
    window.create(sf::VideoMode({800, 600}), "Event Handling");

    while (window.isOpen())
    {
        processEvents();
    }
}