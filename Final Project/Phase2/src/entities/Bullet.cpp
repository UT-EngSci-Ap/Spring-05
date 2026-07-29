#include "entities/Bullet.hpp"
#include "config/Constants.hpp"

Bullet::Bullet(float x, float y) {
    shape.setRadius(Constants::BULLET_RADIUS);
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin({Constants::BULLET_RADIUS, Constants::BULLET_RADIUS});
    shape.setPosition({x, y});
}

void Bullet::update(float dt) {
    shape.move({0.f, -Constants::BULLET_SPEED * dt});
    if (shape.getPosition().y + Constants::BULLET_RADIUS < 0.f)
        dead = true;
}

void Bullet::move(float x, float y) {
    shape.move({x, y});
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();
}
