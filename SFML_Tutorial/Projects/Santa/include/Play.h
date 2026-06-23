#ifndef __PLAY__
#define __PLAY__
#include "State.h"
#include "ResourceManager.h"
#include "Player.h"
#include "BackGround.h"
#include "Obstacle.h"
#include <vector>
#include <memory>

class Play : public State
{
private:
    BackGround back_ground;
    Player player;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    void onKeyPressed(const sf::Event::KeyPressed &);
    void onKeyReleased(const sf::Event::KeyReleased &);

public:
    Play(ResourceManager &, sf::Vector2u);
    void handleEvent(const sf::Event &) override;
    void draw(sf::RenderWindow &) override;
    void update(float dt) override;
    void manageBound();
};

#endif