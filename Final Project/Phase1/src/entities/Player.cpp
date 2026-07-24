#include "entities/Player.hpp"
#include "config/Constants.hpp"
#include "utils/Utils.hpp"

Player::Player(float x, float y) :
    Entity(x, y)
{
    Utils::loadTexture(texture, "assets/right_doodle.png");
    Utils::loadTexture(leftDoodleTexture, "assets/left_doodle.png");
    sprite.setTexture(texture, true);
    float scaleX = Constants::PLAYER_WIDTH / sprite.getLocalBounds().size.x;
    float scaleY = Constants::PLAYER_HEIGHT / sprite.getLocalBounds().size.y;
    sprite.setScale({scaleX, scaleY});
    initPlayer();
}

void Player::handle_input() {

    velocity.x = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = speed;
        sprite.setTexture(texture);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = -speed;
        sprite.setTexture(leftDoodleTexture);
    }
}


void Player::update(float dt) {
    velocity.y += gravity * dt;
    previousPos = sprite.getPosition();
    sprite.move(velocity * dt);
    wrap();
}

void Player::jump() {
    velocity.y = Constants::JUMP_SPEED;
}

void Player::springJump() {
    velocity.y = -800.f;
}


void Player::wrap() {
    float length = sprite.getGlobalBounds().size.x;
    if (sprite.getPosition().x + length < 0) {
        sprite.setPosition({Constants::WINDOW_WIDTH, sprite.getPosition().y});
    }

    if (sprite.getPosition().x > Constants::WINDOW_WIDTH) {
        sprite.setPosition({-length, sprite.getPosition().y});
    }
}

void Player::initPlayer() {
    velocity = sf::Vector2f(0.f, 0.f);
    speed = Constants::HORIZONTAL_SPEED;
    gravity = Constants::GRAVITY;
    score = 0;
    sprite.setPosition(Constants::PLAYER_INITIAL_POS);
}

void Player::addScore(float amount) {
    score += amount;
}

float Player::getVelY() {
    return velocity.y;
}

sf::Vector2f Player::getPrivousPos() {
    return previousPos;
}
