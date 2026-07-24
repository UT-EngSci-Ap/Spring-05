#include "entities/Entity.hpp"

Entity::Entity(float x, float y) : sprite(texture) {
    sprite.setPosition({x, y});
}

void Entity::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Vector2f Entity::getPosition() {
    return sprite.getPosition();
}

sf::FloatRect Entity::getLocalBounds() {
    return sprite.getLocalBounds();
}

sf::FloatRect Entity::getGlobalBounds() {
    return sprite.getGlobalBounds();
}

void Entity::setPosition(float x, float y) {
    sprite.setPosition({x, y});
}
