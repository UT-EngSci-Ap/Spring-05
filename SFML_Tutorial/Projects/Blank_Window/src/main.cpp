#include <SFML/Window.hpp>

int main()
{
    sf::Window window(sf::VideoMode({800, 600}), "Blank Window");

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