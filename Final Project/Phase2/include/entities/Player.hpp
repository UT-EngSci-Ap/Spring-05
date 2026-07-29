#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "entities/Entity.hpp"

class Player : public Entity{
public:
    Player(float x, float y);
    void handle_input();
    void update(float dt) override;
    void render(sf::RenderWindow& window);
    void jump();
    void springJump();
    void monsterBounceJump();
    void wrap();
    void initPlayer();
    void addScore(float amount);
    float getVelY();
    sf::Vector2f getPrivousPos();
    int getScore() { return static_cast<int>(score); }

    // Shooting: held while Space is pressed. Switches to the shooting-pose
    // sprite (with the nose composited on top) and fires bullets at a rate
    // controlled by the caller (difficulty dependent).
    void setShootingInput(bool held);
    bool isShootingPose() const { return shootingPose; }
    bool updateShootTimer(float dt, float cooldown);
    sf::Vector2f getMuzzlePosition() const;

    // Holes ("black hole" effect): once caught, the player freezes and
    // shrinks away over PLAYER_SHRINK_DURATION seconds while being drawn in
    // toward the hole's center, like it's being sucked in.
    void startShrinking(sf::Vector2f targetCenter);
    bool isShrinking() const { return shrinking; }
    bool isConsumed() const { return consumed; }

private:
    sf::Vector2f velocity;
    sf::Vector2f previousPos;
    float speed;
    float gravity;
    float score = 0;
    sf::Texture leftDoodleTexture;
    sf::Texture shootingTexture;
    sf::Texture noseTexture;
    sf::Sprite noseSprite;
    bool facingLeft = false;

    sf::Vector2f normalScale;
    sf::Vector2f shootScale;
    sf::Vector2f noseScale;

    // Fraction of each texture's height that is transparent padding above
    // the actual artwork. The bounding box top isn't always the visual head
    // top (the shooting pose has empty space above the raised arm), so this
    // is measured once at load time and used to find where the head really
    // starts for whichever texture is currently active.
    float rightHeadTopRatio = 0.f;
    float leftHeadTopRatio = 0.f;
    float shootHeadTopRatio = 0.f;

    bool shootingPose = false;
    float shootTimer = 0.f;

    bool shrinking = false;
    bool consumed = false;
    float shrinkTimer = 0.f;
    float shrinkFactor = 1.f;
    sf::Vector2f shrinkStartCenter;
    sf::Vector2f shrinkTarget;

    void applyPoseScale();
    void updateShrink(float dt);
    void updateNosePosition();
};

#endif
