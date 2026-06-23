#ifndef __PLAYER__
#define __PLAYER__
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "Direction.h"

class Player : public Entity
{
private:
    sf::Sprite sprite;
    float move_speed;
    float jump_speed;
    sf::Vector2f acc;
    sf::Vector2f velocity;
    Direction dir;
    bool is_jumping;

public:
    Player(const sf::Texture &, float, float, sf::Vector2f);
    void draw(sf::RenderWindow &) override;
    void setPosition(sf::Vector2f) override;
    void setScale(sf::Vector2f) override;
    sf::FloatRect getGlobalBounds() const override;
    sf::FloatRect getLocalBounds() const override;
    void update(float dt) override;
    sf::Vector2f getPosition() const override;
    void jump();
    void startMoveRight();
    void startMoveLeft();
    void stopMoveRight();
    void stopMoveLeft();
    void releaseJump();
    void stopJump();
};

#endif