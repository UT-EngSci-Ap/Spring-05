#include "platforms/BreakablePlatform.hpp"
#include "config/Constants.hpp"
#include "entities/Player.hpp"
#include "utils/Utils.hpp"

BreakablePlatform::BreakablePlatform(float x, float y)
    : Platform(x, y)
{
    Utils::loadTexture(texture, "assets/images/broken_platform.png");
    sprite.setTexture(texture, true);
    float scaleX = Constants::PLATFORM_WIDTH / sprite.getLocalBounds().size.x;
    float scaleY = Constants::PLATFORM_HEIGHT / sprite.getLocalBounds().size.y;
    sprite.setScale({scaleX, scaleY});
    fallSpeed = 0.f;
    isBroke = false;
}

bool BreakablePlatform::onLandPlayer(Player& player) {
    // A breakable platform crumbles on contact: the player does NOT bounce,
    // they fall through (matches Doodle Jump behaviour).
    (void)player;
    isBroke = true;
    fallSpeed = 200.f;
    return false;
}

void BreakablePlatform::update(float dt) {
    if (isBroke) {
        move(0, fallSpeed * dt);
    }
}

void BreakablePlatform::reset() {
    isBroke = false;
    fallSpeed = 0.f;
}
