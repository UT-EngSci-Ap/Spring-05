#ifndef __OBSTACLE__
#define __OBSTACLE__
#include "MGObj.h"
#include <SFML/Graphics.hpp>
#include "Direction.h"
#include "Collision.h"

class Obstacle : public GObj
{
private:
    sf::Sprite sprite;
    Direction calculateSide(sf::FloatRect, sf::FloatRect);

public:
    Obstacle(sf::Texture &);
    void draw(sf::RenderWindow &) override;
    void setPosition(sf::Vector2f) override;
    void setScale(sf::Vector2f) override;
    sf::FloatRect getGlobalBounds() const override;
    sf::FloatRect getLocalBounds() const override;
    sf::Vector2f getPosition() const override;
    std::optional<Collision> detectCollision(sf::FloatRect);
};

#endif