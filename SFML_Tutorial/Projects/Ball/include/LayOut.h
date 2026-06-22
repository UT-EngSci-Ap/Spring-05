#ifndef __LayOut__
#define __LayOut__
#include <vector>
#include "GObj.h"

namespace LayOut
{
    template <typename SizOPosable>
    void centerVertical(std::vector<std::reference_wrapper<SizOPosable>>, sf::Vector2u, float spacing = 0);

    template <typename SizOPosable>
    void centerVertical(std::vector<std::reference_wrapper<SizOPosable>>, sf::Vector2f, float spacing = 0);

    template <typename Scalable>
    void fill(Scalable &, sf::Vector2f, float percentage = 1);

    template <typename Scalable>
    void fill(Scalable &, sf::Vector2u, float percentage = 1);
};

#endif