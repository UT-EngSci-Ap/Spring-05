#ifndef __ENTITY__
#define __ENTITY__
#include "GObj.h"

class Entity : public GObj
{
private:
public:
    ~Entity() = default;
    virtual void update(float dt) = 0;
};

#endif