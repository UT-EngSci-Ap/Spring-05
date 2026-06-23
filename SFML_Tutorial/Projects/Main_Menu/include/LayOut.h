#ifndef __LAYOUT__
#define __LAYOUT__
#include <vector>
#include "GObj.h"

namespace LayOut
{
    void centerVertical(std::vector<std::reference_wrapper<GObj>>, sf::Vector2u win_size, float spacing);

    void fill(GObj &, sf::Vector2u);
};

#endif