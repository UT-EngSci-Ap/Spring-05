#include "platforms/NormalPlatform.hpp"
#include "config/Constants.hpp"
#include "utils/Utils.hpp"

NormalPlatform::NormalPlatform(float x, float y)
    : Platform(x, y)
{
    Utils::loadTexture(texture, "assets/images/normal_platform.png");
    sprite.setTexture(texture, true);
    float scaleX = Constants::PLATFORM_WIDTH / sprite.getLocalBounds().size.x;
    float scaleY = Constants::PLATFORM_HEIGHT / sprite.getLocalBounds().size.y;
    sprite.setScale({scaleX, scaleY});
}

void NormalPlatform::update(float dt) {
    // Only normal platforms can carry a spring, so the spring animation is
    // advanced here (the base update is pure-virtual and never runs).
    if (spring)
        spring->update(dt);
}
