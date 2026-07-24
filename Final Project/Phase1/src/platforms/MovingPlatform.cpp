#include "platforms/MovingPlatform.hpp"
#include "config/Constants.hpp"
#include "utils/Utils.hpp"


MovingPlatform::MovingPlatform(float x, float y)
    : Platform(x, y)
{
    Utils::loadTexture(texture, "assets/moving_platform.png");
    sprite.setTexture(texture, true);
    float scaleX = Constants::PLATFORM_WIDTH / sprite.getLocalBounds().size.x;
    float scaleY = Constants::PLATFORM_HEIGHT / sprite.getLocalBounds().size.y;
    sprite.setScale({scaleX, scaleY});
    speed = 80.f;
}

void MovingPlatform::update(float dt) {
    move(speed * dt, 0);

    if (getPosition().x < 0 ||
        getPosition().x > Constants::WINDOW_WIDTH - Constants::PLATFORM_WIDTH)
    {
        speed = -speed;
    }
}
