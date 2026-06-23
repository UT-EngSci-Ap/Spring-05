#ifndef __PLAY__
#define __PLAY__
#include "State.h"
#include "ResourceManager.h"
#include "Player.h"
#include "BackGround.h"
#include <list>
#include <memory>
#include "ObstacleContainer.h"
class Play : public State
{
private:
    BackGround back_ground;
    Player player;
    ObstacleContainer obstacle_container;
    sf::View view;
    float max_camera_x;
    void onKeyPressed(const sf::Event::KeyPressed &);
    void onKeyReleased(const sf::Event::KeyReleased &);
    void manageBound();
    void manageXCollision();
    void manageYCollision();
    float getViewLeftBound();
    void handleView();

public:
    Play(ResourceManager &, sf::Vector2u);
    void handleEvent(const sf::Event &) override;
    void draw(sf::RenderWindow &) override;
    void update(float dt) override;
};

#endif