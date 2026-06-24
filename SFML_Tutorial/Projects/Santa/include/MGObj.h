#ifndef __M_G_OBJ__
#define __M_G_OBJ__
#include "GObj.h"

class MGObj : public GObj
{
private:
public:
    ~MGObj() = default;
    virtual void update(float dt) = 0;
};

#endif