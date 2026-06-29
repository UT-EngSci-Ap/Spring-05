#include "../../Santa_Obstacles/include/Play.h"
#include "Player.h"
#include "ResourceManager.h"
#include "LayOut.h"
#include <iostream>
#include "Play.h"

constexpr float GROUND_OFFSET = 30.f;
constexpr float PLAYER_MOVE_SPEED = 0.6f;
constexpr float PLAYER_JUMP_SPEED = 2.5f;
constexpr float PLAYER_GRAVITY = 0.01f;
constexpr float PLAYER_WIDTH = 88.25f;
constexpr float PLAYER_HEIGHT = 140.25f;
constexpr unsigned int OBSTACLE_SIZE = 70;
constexpr float OBSTACLE_DISTANCE_DEGREE = 2.5f;

Play::Play(ResourceManager &resource_manager, sf::Vector2u win_size) : State(win_size),
                                                                       back_ground(resource_manager.getTexture("background.png"),
                                                                                   {0, static_cast<float>(win_size.y) - GROUND_OFFSET}),
                                                                       player(resource_manager.getTexture("player.png"),
                                                                              /*move_speed*/ PLAYER_MOVE_SPEED,
                                                                              /*jump_speed*/ PLAYER_JUMP_SPEED,
                                                                              /*acceleration*/ {0, PLAYER_GRAVITY}),
                                                                       obstacle_container(resource_manager.getTexture("obstacle.png"), win_size,
                                                                                          {OBSTACLE_SIZE, OBSTACLE_SIZE},
                                                                                          OBSTACLE_DISTANCE_DEGREE),
                                                                       max_camera_x(win_size.x / 2.f)

{
    view.setCenter({win_size.x / 2.f,
                    win_size.y / 2.f});
    view.setSize(sf::Vector2f(win_size));

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
    window.setView(window.getDefaultView());
    back_ground.draw(window);

    window.setView(view);
    obstacle_container.draw(window);
    player.draw(window);
}

void Play::update(float dt)
{
    player.moveX(dt);
    manageXCollision();
    player.moveY(dt);
    manageYCollision();
    manageBound();
    handleView();
}

void Play::manageXCollision()
{
    if (auto collision = obstacle_container.detectCollision(player.getGlobalBounds()))
    {
        auto dir = collision->dir;
        if (dir == LEFT)
        {
            player.setPosition({player.getPosition().x - collision->intersection.size.x,
                                player.getPosition().y});
        }

        if (dir == RIGHT)
        {
            player.setPosition({player.getPosition().x + collision->intersection.size.x,
                                player.getPosition().y});
        }
    }
}
void Play::manageYCollision()
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
    }
}
float Play::getViewLeftBound()
{
    auto view_center = view.getCenter();
    auto view_size = view.getSize();
    return view_center.x - view_size.x / 2.f;
}
void Play::manageBound()
{
    auto red_channel = back_ground.getRedChannel();

    if (player.getPosition().x < red_channel.x + getViewLeftBound())
    {
        player.setPosition({red_channel.x + getViewLeftBound(),
                            player.getPosition().y});
    }
    if (player.getPosition().y > red_channel.y)
    {
        player.releaseJump();
        player.setPosition({player.getPosition().x,
                            red_channel.y});
    }
}
void Play::handleView()
{
    float trigg_point = win_size.x / 2.f;
    auto player_x = player.getPosition().x;
    if (player_x > trigg_point)
    {
        max_camera_x = std::max(max_camera_x, player_x);
    }
    view.setCenter({max_camera_x,
                    win_size.y / 2.f});

    obstacle_container.handleView(getViewLeftBound() + back_ground.getRedChannel().x);
}