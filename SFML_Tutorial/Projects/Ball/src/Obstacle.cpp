#include "Obstacle.h"

Obstacle::Obstacle(sf::Texture &t) : sprite(t)
{
}

void Obstacle::update(float dt) {}
void Obstacle::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}
void Obstacle::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}
void Obstacle::setScale(sf::Vector2f scale)
{
    sprite.setScale(scale);
}
sf::FloatRect Obstacle::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}
sf::FloatRect Obstacle::getLocalBounds() const
{
    return sprite.getLocalBounds();
}
