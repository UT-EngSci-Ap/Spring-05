#include "../../Santa_Obstacles/include/Player.h"

Player::Player(const sf::Texture &t, float s, float j, sf::Vector2f a) : sprite(t),
                                                                         move_speed(s),
                                                                         jump_speed(j),
                                                                         acc(a),
                                                                         velocity({0, 0}),
                                                                         dir(NONE),
                                                                         is_jumping(false)
{
    const auto &bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.position.x,
                      bounds.position.y + bounds.size.y});
}

void Player::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}
void Player::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}
void Player::setScale(sf::Vector2f scale)
{
    sprite.setScale(scale);
}
sf::FloatRect Player::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}
sf::FloatRect Player::getLocalBounds() const
{
    return sprite.getLocalBounds();
}
void Player::moveX(float dt)
{
    sf::Vector2f move({velocity.x * dt,
                       0});
    sprite.move(move);
    velocity = {
        velocity.x + acc.x * dt,
        velocity.y};
}
void Player::moveY(float dt)
{
    sprite.move({0,
                 velocity.y * dt});
    velocity = {
        velocity.x,
        velocity.y + acc.y * dt};
}
sf::Vector2f Player::getPosition() const
{
    return sprite.getPosition();
}
void Player::jump()
{
    if (is_jumping)
        return;

    is_jumping = true;
    velocity = {
        velocity.x,
        0 - jump_speed};
}
void Player::startMoveRight()
{
    if (dir == RIGHT)
        return;

    dir = RIGHT;
    velocity = {
        move_speed,
        velocity.y};
}

void Player::startMoveLeft()
{
    if (dir == LEFT)
        return;

    dir = LEFT;
    velocity = {
        0 - move_speed,
        velocity.y};
}

void Player::stopMoveRight()
{
    if (dir != RIGHT)
        return;

    dir = NONE;
    velocity = {0, velocity.y};
}
void Player::stopMoveLeft()
{
    if (dir != LEFT)
        return;
    dir = NONE;
    velocity = {0, velocity.y};
}
void Player::releaseJump()
{
    velocity = {
        velocity.x,
        0};
    is_jumping = false;
}
void Player::stopJump()
{
    if (velocity.y < 0)
    {
        velocity = {velocity.x, 0};
    }
}