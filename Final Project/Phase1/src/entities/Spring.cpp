#include "entities/Spring.hpp"
#include "entities/Player.hpp"
#include "config/Constants.hpp"
#include "utils/Utils.hpp"


Spring::Spring(float x, float y) :
    Entity(x, y)
{
    Utils::loadTexture(texture, "assets/spring_sprite.png");
    sprite.setTexture(texture, true);
    sprite.setTextureRect(sf::IntRect({0, 0}, {25, 16}));
    float scaleX = Constants::SPRING_WIDTH / sprite.getLocalBounds().size.x;
    float scaleY = Constants::SPRING_HEIGHT / sprite.getLocalBounds().size.y;
    sprite.setScale({scaleX, scaleY});
    sf::FloatRect lb = getLocalBounds();
    sprite.setOrigin({
        lb.position.x + lb.size.x / 2.f,
        lb.position.y + lb.size.y / 2.f
    });
}

void Spring::update(float dt) {
    if (activated) {
        timer += dt;
        if (timer >= 0.01f) {
            activated = false;
            sprite.setTextureRect(sf::IntRect({0, 0}, {25, 16}));
        }
    }
}

void Spring::handleCollision(Player& player) {
    bool isColliding =
        getGlobalBounds().findIntersection(player.getGlobalBounds()).has_value();

    float playerPrevBottom =
        player.getPrivousPos().y + player.getGlobalBounds().size.y;
    float springTop = getGlobalBounds().position.y;

    if (player.getVelY() > 0 && playerPrevBottom < springTop &&
        isColliding && !wasColliding && !activated) {
        player.springJump();
        activate();
    }

    wasColliding = isColliding;
}

void Spring::move(float x, float y) {
    sprite.move({x, y});
}

void Spring::reset() {
    wasColliding = false;
    activated = false;
    timer = 0.f;
    sprite.setTextureRect(sf::IntRect({0, 0}, {25, 16}));
}

void Spring::activate() {
    activated = true;
    timer = 0.f;
    sprite.setTextureRect(sf::IntRect({0, 20}, {25, 16}));
}
