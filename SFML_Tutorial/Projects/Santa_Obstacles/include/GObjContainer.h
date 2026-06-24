#ifndef __GOBJ_CONTAINER__
#define __GOBJ_CONTAINER__
#include <SFML/Graphics.hpp>

class GObjContainer
{
private:
public:
    virtual ~GObjContainer() = default;
    virtual void draw(sf::RenderWindow &) = 0;
};

#endif