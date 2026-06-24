#include "game.h"
#include <SFML/Window.hpp>
#include <iostream>

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;

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
        },
        [&](const sf::Event::KeyPressed &e)
        {
            onKeyPressed(e);
        },
        [&](const sf::Event::KeyReleased &e)
        {
            onKeyReleased(e);
        }

    );
}

void Game::onKeyPressed(const sf::Event::KeyPressed &event)
{
    if (event.scancode == sf::Keyboard::Scancode::Escape)
    {
        window.close();
    }
    std::cout << "A key is pressed\n";
}
void Game::onKeyReleased(const sf::Event::KeyReleased &)
{
    std::cout << "The key is released\n";
}
void Game::onMouseButtonPressed(const sf::Event::MouseButtonPressed &event)
{
    std::cout << "Mouse position is: (" << mouse_position.first << ", " << mouse_position.second << ")\n";
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
    window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Event Handling");

    while (window.isOpen())
    {
        processEvents();
    }
}