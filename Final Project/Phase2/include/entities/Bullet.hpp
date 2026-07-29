#pragma once

#include <SFML/Graphics.hpp>

// A green projectile fired from the player's nose while shooting. There is no
// dedicated sprite asset for it, so it is rendered as a simple filled circle
// rather than following the Entity texture/sprite pattern. Bullets only ever
// interact with monsters - they pass straight through platforms.
class Bullet {
public:
    Bullet(float x, float y);
    void update(float dt);
    void move(float x, float y);
    void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

    void kill() { dead = true; }
    bool isDead() const { return dead; }

private:
    sf::CircleShape shape;
    bool dead = false;
};
