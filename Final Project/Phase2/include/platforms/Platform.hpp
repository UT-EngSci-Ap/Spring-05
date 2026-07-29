#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "entities/Spring.hpp"

class Player;

class Platform {
public:
    Platform(float x, float y);
    virtual ~Platform();
    virtual void update(float dt) = 0;
    void render(sf::RenderWindow& window);
    void move(float x, float y);
    // Returns true if landing caused a jump bounce (used to trigger the jump
    // sound). BreakablePlatform overrides this to return false since it
    // crumbles instead of bouncing the player.
    virtual bool onLandPlayer(Player& player);
    virtual void reset();
    // Breakable platforms crumble on contact, so they can never be relied on as
    // part of the guaranteed climb path. Generation anchors only to solid ones.
    virtual bool isBreakable() const { return false; }
    void setOnSpring();
    void handleSpringCollision(Player& player);
    sf::FloatRect getBounds();
    sf::Vector2f getPosition();
    float getWidth();
    void setPosition(float x, float y);

protected:
    sf::Texture texture;
    sf::Sprite sprite;
    std::unique_ptr<Spring> spring;
};


#endif
