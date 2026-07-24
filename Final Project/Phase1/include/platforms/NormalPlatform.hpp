#pragma once

#include "platforms/Platform.hpp"


class NormalPlatform : public Platform {
public:
    NormalPlatform(float x, float y);
    void update(float dt) override;
};
