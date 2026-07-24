#pragma once

#include "SFML/Graphics.hpp"
#include "entities/Entity.hpp"

class Player;


class Spring : public Entity{
public:
    Spring(float x, float y);
    void update(float dt);
    void move(float x, float y);
    void handleCollision(Player& player);
    void reset();
    void activate();
private:
    bool wasColliding = false;
    bool activated = false;
    float timer = 0.f;


};