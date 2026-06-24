#ifndef __OBSTACLE_CONTAINER__
#define __OBSTACLE_CONTAINER__
#include "GObjContainer.h"
#include "Obstacle.h"
#include "Player.h"
#include <list>
#include <memory>
#include "Collision.h"

class ObstacleContainer : public GObjContainer
{
private:
    std::list<std::unique_ptr<Obstacle>> obstacles;
    sf::Texture texture;
    sf::Vector2u win_size;
    sf::Vector2u obstacle_size;
    float distance_degree;
    void spawnObstacle();

public:
    ObstacleContainer(sf::Texture &, sf::Vector2u, sf::Vector2u, float);
    void draw(sf::RenderWindow &) override;
    void handleView(float);
    std::optional<Collision> detectCollision(sf::FloatRect);
};

#endif