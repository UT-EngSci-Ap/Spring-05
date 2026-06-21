#ifndef __BUTTON__
#define __BUTTON__
#include "GObj.h"

class Button : public GObj
{
public:
    Button(const sf::Texture &t) : GObj(t) {}
    bool isHovered(const sf::Vector2i &) const;
};

#endif