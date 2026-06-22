#ifndef __GOBJS__
#define __GOBJS__
#include <SFML/Graphics.hpp>

class GObj
{
protected:
public:
    virtual ~GObj() = default;
    virtual void draw(sf::RenderWindow &) = 0;
    virtual void setPosition(sf::Vector2f) = 0;
    virtual void setScale(sf::Vector2f) = 0;
    virtual sf::FloatRect getGlobalBounds() const = 0;
    virtual sf::FloatRect getLocalBounds() const = 0;
};

#endif