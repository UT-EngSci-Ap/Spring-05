#include "Play.h"
#include "Player.h"
#include "ResourceManager.h"
#include "LayOut.h"

constexpr float GROUND_OFFSET = 30.f;
constexpr float PLAYER_MOVE_SPEED = 0.6f;
constexpr float PLAYER_JUMP_SPEED = 3.f;
constexpr float PLAYER_GRAVITY = 0.02f;
constexpr float PLAYER_WIDTH = 88.25f;
constexpr float PLAYER_HEIGHT = 140.25f;

Play::Play(ResourceManager &resource_manager, sf::Vector2u win_size) : State(win_size),
                                                                       back_ground(resource_manager.getTexture("background.png"),
                                                                                   {0, static_cast<float>(win_size.y) - GROUND_OFFSET}),
                                                                       player(resource_manager.getTexture("player.png"),
                                                                              /*move_speed*/ PLAYER_MOVE_SPEED,
                                                                              /*jump_speed*/ PLAYER_JUMP_SPEED,
                                                                              /*acceleration*/ {0, PLAYER_GRAVITY})

{
    LayOut::fill(back_ground, win_size);
    LayOut::fill(player, sf::Vector2f({PLAYER_WIDTH, PLAYER_HEIGHT}));
    player.setPosition({0, static_cast<float>(win_size.y) - GROUND_OFFSET});
}

void Play::onKeyPressed(const sf::Event::KeyPressed &event)
{
    if (event.scancode == sf::Keyboard::Scancode::Right)
    {
        player.startMoveRight();
    }
    if (event.scancode == sf::Keyboard::Scancode::Left)
    {
        player.startMoveLeft();
    }
    if (event.scancode == sf::Keyboard::Scancode::Space)
    {
        player.jump();
    }
}

void Play::onKeyReleased(const sf::Event::KeyReleased &event)
{
    if (event.scancode == sf::Keyboard::Scancode::Right)
    {
        player.stopMoveRight();
    }
    if (event.scancode == sf::Keyboard::Scancode::Left)
    {
        player.stopMoveLeft();
    }
    if (event.scancode == sf::Keyboard::Scancode::Space)
    {
        player.releaseJump();
    }
}

void Play::handleEvent(const sf::Event &event)
{
    if (event.is<sf::Event::Closed>())
    {
        transition = EXIT;
    }
    else if (const auto *e = event.getIf<sf::Event::KeyPressed>())
    {
        onKeyPressed(*e);
    }
    else if (const auto *e = event.getIf<sf::Event::KeyReleased>())
    {
        onKeyReleased(*e);
    }
}

void Play::draw(sf::RenderWindow &window)
{
    back_ground.draw(window);
    player.draw(window);
}

void Play::update(float dt)
{
    player.update(dt);
    manageBound();
}
#include <iostream>
void Play::manageBound()
{
    auto red_channel = back_ground.getRedChannel();
    if (player.getPosition().x < red_channel.x)
    {
        std::cout << "x collision\n";
        player.setPosition({red_channel.x,
                            player.getPosition().y});
    }
    if (player.getPosition().y > red_channel.y)
    {
        std::cout << "y collision\n";
        player.setPosition({player.getPosition().x,
                            red_channel.y});
    }
}
