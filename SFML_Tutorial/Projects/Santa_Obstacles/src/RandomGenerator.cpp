#include "RandomGenerator.h"

constexpr float X_BOUND_MULT = 2.f;

std::mt19937 RandomGenerator::gen(std::random_device{}());

sf::Vector2f RandomGenerator::randomObstaclePosition(sf::Vector2u obstacle_size,
                                                     float x_degree,
                                                     float y_limit)
{
    float min_x = obstacle_size.x * x_degree;
    float max_x = obstacle_size.x * x_degree * X_BOUND_MULT;

    std::uniform_real_distribution<float> dist_x(min_x, max_x);
    float rand_x = dist_x(gen);

    float min_y = 0.f;
    float max_y = y_limit - obstacle_size.y;

    std::uniform_real_distribution<float> dist_y(min_y, max_y);
    float rand_y = dist_y(gen);

    return sf::Vector2f(rand_x, rand_y);
}