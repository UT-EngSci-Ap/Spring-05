#include "GObj.h"
#include <SFML/Graphics.hpp>

void GObj::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}