#include <SFML/Window.hpp>

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;

int main()
{
    sf::Window window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Blank Window");

    while (window.isOpen())
    {

        while (const std::optional<sf::Event> event = window.pollEvent())
        {

            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }

    return 0;
}