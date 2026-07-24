#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "entities/Entity.hpp"

class Player : public Entity{
public:
    Player(float x, float y);
    void handle_input();
    void update(float dt) override;
    void jump();
    void springJump();
    void wrap();
    void initPlayer();
    void addScore(float amount);
    float getVelY();
    sf::Vector2f getPrivousPos();
    int getScore() { return static_cast<int>(score); }
private:
    sf::Vector2f velocity;
    sf::Vector2f previousPos;
    float speed;
    float gravity;
    float score = 0;
    sf::Texture leftDoodleTexture;

};

#endif
