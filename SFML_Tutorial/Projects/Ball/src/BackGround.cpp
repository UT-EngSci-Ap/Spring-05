#include "BackGround.h"
#include <SFML/Graphics.hpp>
#include "ResourceManaegr.h"
BackGround::BackGround(const sf::Texture &t) : sprite(t) {}

void BackGround::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void BackGround::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}

void BackGround::setScale(sf::Vector2f scale)
{
    sprite.setScale(scale);
}

sf::FloatRect BackGround::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}