#ifndef __GOBJS__
#define __GOBJS__
#include <SFML/Graphics.hpp>

class GObj
{
protected:
    sf::Sprite sprite;

public:
    virtual ~GObj() = default;
    GObj(const sf::Texture &t) : sprite(t) {}
    void draw(sf::RenderWindow &);
    void setPosition(sf::Vector2f);
    void setScale(sf::Vector2f);
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2i getSize() const;
};

#endif