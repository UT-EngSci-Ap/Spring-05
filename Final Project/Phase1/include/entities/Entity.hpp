# pragma once

#include "SFML/Graphics.hpp"


class Entity {
public:
    Entity(float x, float y);   
    virtual ~Entity() = default;
    virtual void update(float dt) = 0;
    void render(sf::RenderWindow& window);

    // setter and getter
    sf::Vector2f getPosition();
    sf::FloatRect getLocalBounds();
    sf::FloatRect getGlobalBounds();
    void setPosition(float x, float y);
protected:
    sf::Texture texture;
    sf::Sprite sprite;
};