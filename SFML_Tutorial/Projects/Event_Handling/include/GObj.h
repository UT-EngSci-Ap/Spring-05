#ifndef __GOBJS__
#define __GOBJS__
#include <SFML/Graphics.hpp>

class GObjs
{
private:
    sf::Sprite sprite;

public:
    virtual ~GObjs() = default;
    GObjs(const sf::Sprite t) : sprite(t) {}
    void draw(sf::RenderWindow &);
    void setPosition(const sf::Vector2f &);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
};

#endif