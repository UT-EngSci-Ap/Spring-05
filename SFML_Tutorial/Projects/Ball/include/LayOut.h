#ifndef __LayOut__
#define __LayOut__
#include <vector>
#include "GObj.h"

namespace LayOut
{
    void centerVertical(std::vector<std::reference_wrapper<GObj>>, sf::Vector2u winSize, float spacing);

    void fill(GObj &, sf::Vector2u);
};

#endif