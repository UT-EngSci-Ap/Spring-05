#include <SFML/Window.hpp>

int main()
{
    sf::Window window;
    window.create(sf::VideoMode({800, 600}), "Event Handler");
    const auto onClose = [&window](const sf::Event::Closed &)
    {
        window.close();
    };
    const auto onKeyPressed = [&window](const sf::Event::KeyPressed &keyPressed)
    {
        if (keyPressed.scancode == sf::Keyboard::Scancode::Escape)
            window.close();
    };

    while (window.isOpen())
    {
        window.handleEvents(onClose, onKeyPressed);
    }
}