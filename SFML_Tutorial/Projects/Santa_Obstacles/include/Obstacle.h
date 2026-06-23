#ifndef __OBSTACLE__
#define __OBSTACLE__
#include "Entity.h"
#include <SFML/Graphics.hpp>

class Obstacle : public Entity
{
private:
    sf::Sprite sprite;

public:
    Obstacle(sf::Texture &);
    void update(float dt) override;
    void draw(sf::RenderWindow &) override;
    void setPosition(sf::Vector2f) override;
    void setScale(sf::Vector2f) override;
    sf::FloatRect getGlobalBounds() const override;
    sf::FloatRect getLocalBounds() const override;
};

#endif