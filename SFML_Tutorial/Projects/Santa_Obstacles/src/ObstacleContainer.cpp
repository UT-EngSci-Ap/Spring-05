#include "ObstacleContainer.h"
#include "RandomGenerator.h"
#include "LayOut.h"
constexpr int UPPER_BOUND = 5;

ObstacleContainer::ObstacleContainer(sf::Texture &t, sf::Vector2u w, sf::Vector2u s, float d) : texture(t), win_size(w), obstacle_size(s), distance_degree(d)
{
    for (int i = 0; i < UPPER_BOUND; ++i)
    {
        spawnObstacle();
    }
}
void ObstacleContainer::spawnObstacle()
{
    auto new_obs = std::make_unique<Obstacle>(texture);
    LayOut::fill(*new_obs, obstacle_size);
    float last_x = 0;
    if (!obstacles.empty())
    {
        last_x = obstacles.back()->getPosition().x;
    }
    auto distance = RandomGenerator::randomObstaclePosition(obstacle_size, distance_degree, win_size.y);
    new_obs->setPosition({last_x + distance.x,
                          distance.y});

    obstacles.push_back(std::move(new_obs));
}

void ObstacleContainer::draw(sf::RenderWindow &window)
{
    for (auto &obstacle : obstacles)
    {
        obstacle->draw(window);
    }
}

void ObstacleContainer::update(float dt)
{
    auto first_bound = obstacles.front()->getGlobalBounds();
    if (first_bound.position.x + first_bound.size.x < 0)
    {
        obstacles.pop_front();
        spawnObstacle();
    }
}

std::optional<Collision> ObstacleContainer::detectCollision(sf::FloatRect player_bounds)
{
    for (auto &obstacle : obstacles)
    {
        if (auto collision = obstacle->detectCollision(player_bounds))
        {
            return collision;
        }
    }
    return std::nullopt;
}
