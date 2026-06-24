#ifndef __RANDOM_GENERATOR__
#define __RANDOM_GENERATOR__

#include <random>
#include <SFML/Graphics.hpp>

class RandomGenerator
{
private:
    static std::mt19937 gen;

public:
    static sf::Vector2f randomObstaclePosition(sf::Vector2u obstacle_size,
                                               float x_degree,
                                               float y_limit);
};

#endif