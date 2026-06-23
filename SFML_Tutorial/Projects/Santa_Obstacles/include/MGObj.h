#ifndef __M_G_OBJ__
#define __M_G_OBJ__
#include "GObj.h"

class MGObj : public GObj
{
private:
public:
    ~MGObj() = default;
    virtual void moveX(float dt) = 0;
    virtual void moveY(float dt) = 0;
};

#endif