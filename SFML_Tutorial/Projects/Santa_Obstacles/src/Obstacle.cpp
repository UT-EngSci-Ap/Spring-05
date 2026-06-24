#include "Obstacle.h"

Obstacle::Obstacle(sf::Texture &t) : sprite(t)
{
}

Direction Obstacle::calculateSide(sf::FloatRect main_rect, sf::FloatRect sub_rect)
{
    float overlap_left = (sub_rect.position.x + sub_rect.size.x) - main_rect.position.x;
    float overlap_right = (main_rect.position.x + main_rect.size.x) - sub_rect.position.x;
    float overlap_up = (sub_rect.position.y + sub_rect.size.y) - main_rect.position.y;
    float overlap_down = (main_rect.position.y + main_rect.size.y) - sub_rect.position.y;

    if (overlap_left < overlap_right &&
        overlap_left < overlap_up &&
        overlap_left < overlap_down)
    {
        return LEFT;
    }
    else if (overlap_right < overlap_up &&
             overlap_right < overlap_down)
    {
        return RIGHT;
    }
    else if (overlap_up < overlap_down)
    {
        return UP;
    }
    else
    {
        return DOWN;
    }
}

void Obstacle::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}
void Obstacle::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}
void Obstacle::setScale(sf::Vector2f scale)
{
    sprite.setScale(scale);
}
sf::FloatRect Obstacle::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}
sf::FloatRect Obstacle::getLocalBounds() const
{
    return sprite.getLocalBounds();
}

sf::Vector2f Obstacle::getPosition() const
{
    return sprite.getPosition();
}

std::optional<Collision> Obstacle::detectCollision(sf::FloatRect player_bounds)
{
    auto obstacle_bounds = sprite.getGlobalBounds();
    if (const auto intersection = obstacle_bounds.findIntersection(player_bounds))
    {
        return Collision{calculateSide(obstacle_bounds, *intersection), *intersection};
    }
    return std::nullopt;
}
