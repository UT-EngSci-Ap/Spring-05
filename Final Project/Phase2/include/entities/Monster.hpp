#pragma once

#include "SFML/Graphics.hpp"
#include "entities/Entity.hpp"

class Player;

class Monster : public Entity {
public:
    // Result of a player touching a monster:
    //  - None:   no overlap this frame.
    //  - Bounce: player landed cleanly on top -> springs the player upward.
    //  - Kill:   touched from any other direction -> game over.
    enum class CollisionResult { None, Bounce, Kill };

    Monster(float x, float y, int health, float speed, bool blueVariant);
    void update(float dt) override;
    void move(float x, float y);

    // Recycles a dead/off-screen monster into a fresh one at a new spot.
    void reset(float x, float y, int health, float speed);

    CollisionResult resolvePlayerCollision(Player& player);

    // Applies one bullet hit. The monster dies once its health drops to 0.
    void takeHit();
    bool isDead() const { return dead; }

private:
    int health;
    float speed;
    bool dead = false;
};
