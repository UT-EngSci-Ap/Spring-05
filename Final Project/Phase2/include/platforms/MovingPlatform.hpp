#pragma once

#include "platforms/Platform.hpp"

class MovingPlatform : public Platform {
public:
    // speedMultiplier scales the base patrol speed (used to make moving
    // platforms faster on Medium/Hard difficulty).
    MovingPlatform(float x, float y, float speedMultiplier = 1.f);
    void update(float dt) override;

private:
    float speed;
};
