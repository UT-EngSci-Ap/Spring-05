#pragma once

#include "platforms/Platform.hpp"

class MovingPlatform : public Platform {
public:
    MovingPlatform(float x, float y);
    void update(float dt) override;

private:
    float speed;
};
