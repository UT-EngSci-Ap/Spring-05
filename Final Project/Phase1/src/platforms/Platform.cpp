#include "platforms/Platform.hpp"
#include "entities/Player.hpp"
#include "config/Constants.hpp"
#include "entities/Spring.hpp"

Platform::Platform(float x, float y) : sprite(texture) {
    sprite.setPosition({x, y});
}


Platform::~Platform() = default;


void Platform::render(sf::RenderWindow& window) {
    window.draw(sprite);
    if (spring)
        spring->render(window);
}

sf::FloatRect Platform::getBounds() {
    return sprite.getGlobalBounds();
}

void Platform::move(float x, float y) {
    sprite.move({x, y});
    if (spring) {
        spring->move(x, y);
    }
}

void Platform::reset() {
    if (spring) {
        spring->reset();
    }
}

void Platform::setOnSpring() {
    sf::FloatRect bounds = getBounds();
    float springX = bounds.position.x + bounds.size.x / 2.f;
    float springY = bounds.position.y - Constants::SPRING_HEIGHT / 2.f;
    spring = std::make_unique<Spring>(springX, springY);
}

void Platform::handleSpringCollision(Player& player) {
    if (spring) {
        spring->handleCollision(player);
    }
}

void Platform::onLandPlayer(Player& player) {
    player.jump();
}

sf::Vector2f Platform::getPosition() {
    return sprite.getPosition();
}

float Platform::getWidth() {
    return texture.getSize().x;
}

void Platform::setPosition(float x, float y) {
    sprite.setPosition({x, y});

    if (spring) {
        sf::FloatRect bounds = getBounds();
        float springX = bounds.position.x + bounds.size.x / 2.f;
        float springY = bounds.position.y - bounds.size.y / 2.f;
        spring->setPosition(springX, springY);
    }
}
