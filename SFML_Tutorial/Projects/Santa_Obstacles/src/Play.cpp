#include "../../Santa_Obstacles/include/Play.h"
#include "Player.h"
#include "ResourceManager.h"
#include "LayOut.h"
#include <iostream>

Play::Play(ResourceManager &resource_manager, sf::Vector2u win_size) : State(win_size),
                                                                       back_ground(resource_manager.getTexture("background.png"),
                                                                                   {0, static_cast<float>(win_size.y) - 30}),
                                                                       player(resource_manager.getTexture("player.png"),
                                                                              /*move_speed*/ 1,
                                                                              /*jump_speed*/ 4,
                                                                              /*acceleration*/ {0, 0.02}),
                                                                       obstacle_container(resource_manager.getTexture("obstacle.png"), win_size,
                                                                                          {70, 70},
                                                                                          2.5)

{
    LayOut::fill(back_ground, win_size);
    LayOut::fill(player, sf::Vector2f({88.25, 140.25}));
    player.setPosition({0, static_cast<float>(win_size.y) - 30});
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
    obstacle_container.draw(window);
    player.draw(window);
}

void Play::update(float dt)
{
    auto last_pos = player.getPosition();
    player.update(dt);
    manageCollision(last_pos);
    manageBound();
}

void Play::manageCollision(sf::Vector2f last_pos)
{
    if (auto collision = obstacle_container.detectCollision(player.getGlobalBounds()))
    {
        auto dir = collision->dir;
        if (dir == UP)
        {
            player.releaseJump();
            player.setPosition({player.getPosition().x,
                                player.getPosition().y - collision->intersection.size.y});
        }
        if (dir == DOWN)
        {
            player.stopJump();
            player.setPosition({player.getPosition().x,
                                player.getPosition().y + collision->intersection.size.y});
        }
        if (dir == LEFT)
        {
            player.stopMoveRight();

            player.setPosition({player.getPosition().x - collision->intersection.size.x,
                                player.getPosition().y});
        }

        if (dir == RIGHT)
        {
            player.stopMoveLeft();

            player.setPosition({player.getPosition().x + collision->intersection.size.x,
                                player.getPosition().y});
        }
    }
}
void Play::manageBound()
{
    auto red_channel = back_ground.getRedChannel();
    if (player.getPosition().x < red_channel.x)
    {
        player.setPosition({red_channel.x,
                            player.getPosition().y});
    }
    if (player.getPosition().y > red_channel.y)
    {
        player.releaseJump();
        player.setPosition({player.getPosition().x,
                            red_channel.y});
    }
}
