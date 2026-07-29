#include "entities/Hole.hpp"
#include "config/Constants.hpp"
#include "utils/Utils.hpp"

Hole::Hole(float x, float y, Size sizeIn) : Entity(x, y), size(sizeIn) {
    Utils::loadTexture(smallTexture, "assets/images/hole.png");
    Utils::loadTexture(bigTexture, "assets/images/hole@2x.png");
    applyTextureForSize();
}

void Hole::applyTextureForSize() {
    sf::Texture& tex = (size == Size::Small) ? smallTexture : bigTexture;
    sprite.setTexture(tex, true);
    float target = (size == Size::Small) ? Constants::HOLE_SMALL_SIZE : Constants::HOLE_BIG_SIZE;
    float scaleX = target / sprite.getLocalBounds().size.x;
    float scaleY = target / sprite.getLocalBounds().size.y;
    sprite.setScale({scaleX, scaleY});
}

void Hole::move(float x, float y) {
    sprite.move({x, y});
}

void Hole::reset(float x, float y, Size sizeIn) {
    size = sizeIn;
    applyTextureForSize();
    sprite.setPosition({x, y});
}
