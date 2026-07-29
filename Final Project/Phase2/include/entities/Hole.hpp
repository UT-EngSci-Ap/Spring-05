#pragma once

#include "SFML/Graphics.hpp"
#include "entities/Entity.hpp"

// A black-hole hazard used in Hard mode. Touching it starts shrinking the
// player away (see Player::startShrinking) instead of an immediate death.
class Hole : public Entity {
public:
    enum class Size { Small, Big };

    Hole(float x, float y, Size size);
    void update(float dt) override {}
    void move(float x, float y);

    // Recycles a hole that scrolled off-screen into a new spot/size.
    void reset(float x, float y, Size size);

    Size getSize() const { return size; }

private:
    Size size;
    sf::Texture smallTexture;
    sf::Texture bigTexture;

    void applyTextureForSize();
};
