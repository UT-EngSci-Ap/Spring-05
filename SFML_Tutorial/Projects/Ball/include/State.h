#ifndef __STATE__
#define __STATE__
#include <SFML/Graphics.hpp>
#include "Transition.h"

class State
{
protected:
    Transition transition;
    sf::Vector2u win_size;

public:
    State(sf::Vector2u);
    virtual void handleEvent(const sf::Event &) = 0;
    virtual void draw(sf::RenderWindow &) = 0;
    Transition getTransition() const;
};

#endif