#include "State.h"

State::State(sf::Vector2u w) : win_size(w)
{
}

Transition State::getTransition() const
{
    return transition;
}