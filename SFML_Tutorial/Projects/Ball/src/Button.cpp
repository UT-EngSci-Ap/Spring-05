#include "Button.h"
bool Button::isHovered(const sf::Vector2i &mouse_position) const
{
    return sprite.getGlobalBounds().contains(sf::Vector2f(mouse_position));
}
