#ifndef __COLLISION__
#define __COLLISION__
#include "Direction.h"
#include <SFML/Graphics.hpp>

struct Collision
{
    Direction dir;
    sf::FloatRect intersection;
};

#endif