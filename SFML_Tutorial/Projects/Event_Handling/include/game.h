#ifndef __GAME__
#define __GAME__

#include <SFML/Window.hpp>

class Game
{
private:
    sf::Window window;
    std::pair<int, int> mouse_position;
    void processEvents();
    void onClosed();
    void onMouseMoved(const sf::Event::MouseMoved &);
    void onMouseButtonPressed(const sf::Event::MouseButtonPressed &);
    void onKeyPressed(const sf::Event::KeyPressed &);
    void onKeyReleased(const sf::Event::KeyReleased &);

public:
    Game() : mouse_position({0, 0}) {}
    void run();
};

#endif