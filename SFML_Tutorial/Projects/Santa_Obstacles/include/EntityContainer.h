#ifndef __ENTITY_CONTAINER__
#define __ENTITY_CONTAINER__
#include "GObjContainer.h"

class EntityContainer : public GObjContainer
{
private:
public:
    virtual void update(float dt) = 0;
};

#endif