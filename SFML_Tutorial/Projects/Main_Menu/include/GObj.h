#ifndef __GOBJS__
#define __GOBJS__
#include <SFML/Graphics.hpp>

class GObj
{
protected:
    sf::Sprite sprite;

public:
    virtual ~GObj() = default;
    GObj(const sf::Sprite t) : sprite(t) {}
    void draw(sf::RenderWindow &);
};

#endif