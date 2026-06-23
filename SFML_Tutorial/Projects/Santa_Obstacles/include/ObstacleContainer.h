#ifndef __OBSTACLE_CONTAINER__
#define __OBSTACLE_CONTAINER__
#include "EntityContainer.h"

class ObstacleContainer : public EntityContainer
{
private:
public:
    void draw(sf::RenderWindow &) override;
    void update(float dt) override;
};

#endif