#include "GObj.h"
#include <SFML/Graphics.hpp>

void GObjs::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void GObjs::setPosition(const sf::Vector2f &position)
{
    sprite.setPosition(position);
}

sf::Vector2f GObjs::getPosition() const
{
    return sprite.getPosition();
}

sf::FloatRect GObjs::getBounds() const
{
    return sprite.getLocalBounds();
}