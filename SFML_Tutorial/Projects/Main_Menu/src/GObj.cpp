#include "GObj.h"
#include <SFML/Graphics.hpp>

void GObj::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void GObj::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}

void GObj::setScale(sf::Vector2f scale)
{
    sprite.setScale(scale);
}

sf::FloatRect GObj::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}

sf::Vector2i GObj::getSize() const
{
    auto bounds = sprite.getGlobalBounds();

    return {
        static_cast<int>(bounds.size.x),
        static_cast<int>(bounds.size.y)};
}
