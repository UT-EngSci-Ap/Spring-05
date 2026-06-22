#ifndef __PLAY__
#define __PLAY__
#include "State.h"

class Play : public State
{
private:
public:
    Play(sf::Vector2u);
    void handleEvent(const sf::Event &) override;
    void draw(sf::RenderWindow &) override;
};

#endif