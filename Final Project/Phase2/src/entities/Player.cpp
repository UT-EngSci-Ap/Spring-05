#include "entities/Player.hpp"
#include "config/Constants.hpp"
#include "utils/Utils.hpp"
#include <algorithm>

Player::Player(float x, float y) :
    Entity(x, y),
    noseSprite(noseTexture)
{
    Utils::loadTexture(texture, "assets/images/right_doodle.png");
    Utils::loadTexture(leftDoodleTexture, "assets/images/left_doodle.png");
    Utils::loadTexture(shootingTexture, "assets/images/Shooting@Pose.png");
    Utils::loadTexture(noseTexture, "assets/images/Nose.png");

    sprite.setTexture(texture, true);
    normalScale = {
        Constants::PLAYER_WIDTH / sprite.getLocalBounds().size.x,
        Constants::PLAYER_HEIGHT / sprite.getLocalBounds().size.y
    };

    noseSprite.setTexture(noseTexture, true);
    noseScale = {
        Constants::NOSE_WIDTH / noseSprite.getLocalBounds().size.x,
        Constants::NOSE_HEIGHT / noseSprite.getLocalBounds().size.y
    };

    // Measure how much transparent padding sits above the actual artwork in
    // each texture, so the nose can be anchored to the real head position
    // rather than assuming the sprite's bounding box top is always the head.
    rightHeadTopRatio = Utils::opaqueTopRatio(texture);
    leftHeadTopRatio  = Utils::opaqueTopRatio(leftDoodleTexture);
    shootHeadTopRatio = Utils::opaqueTopRatio(shootingTexture);

    // The shooting-pose art has different native proportions than the doodle
    // sprites; scale it independently so the player's overall size (and thus
    // its collision bounds) stays the same regardless of pose.
    sf::Sprite shootProbe(shootingTexture);
    shootScale = {
        Constants::PLAYER_WIDTH / shootProbe.getLocalBounds().size.x,
        Constants::PLAYER_HEIGHT / shootProbe.getLocalBounds().size.y
    };

    initPlayer();
}

void Player::handle_input() {
    if (shrinking) {
        velocity.x = 0.f;
        return;
    }

    velocity.x = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = speed;
        facingLeft = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = -speed;
        facingLeft = true;
    }

    setShootingInput(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space));

    if (!shootingPose) {
        sprite.setTexture(facingLeft ? leftDoodleTexture : texture, true);
    }
}


void Player::update(float dt) {
    if (shrinking) {
        updateShrink(dt);
        updateNosePosition();
        return;
    }

    velocity.y += gravity * dt;
    previousPos = sprite.getPosition();
    sprite.move(velocity * dt);
    wrap();
    updateNosePosition();
}

void Player::jump() {
    velocity.y = Constants::JUMP_SPEED;
}

void Player::springJump() {
    velocity.y = -800.f;
}

void Player::monsterBounceJump() {
    velocity.y = Constants::MONSTER_BOUNCE_SPEED;
}


void Player::wrap() {
    float length = sprite.getGlobalBounds().size.x;
    if (sprite.getPosition().x + length < 0) {
        sprite.setPosition({Constants::WINDOW_WIDTH, sprite.getPosition().y});
    }

    if (sprite.getPosition().x > Constants::WINDOW_WIDTH) {
        sprite.setPosition({-length, sprite.getPosition().y});
    }
}

void Player::initPlayer() {
    velocity = sf::Vector2f(0.f, 0.f);
    speed = Constants::HORIZONTAL_SPEED;
    gravity = Constants::GRAVITY;
    score = 0;
    facingLeft = false;
    shootingPose = false;
    shootTimer = 0.f;
    shrinking = false;
    consumed = false;
    shrinkTimer = 0.f;
    shrinkFactor = 1.f;

    sprite.setTexture(texture, true);
    sprite.setPosition(Constants::PLAYER_INITIAL_POS);
    applyPoseScale();
    updateNosePosition();
}

void Player::addScore(float amount) {
    score += amount;
}

float Player::getVelY() {
    return velocity.y;
}

sf::Vector2f Player::getPrivousPos() {
    return previousPos;
}

void Player::render(sf::RenderWindow& window) {
    // Drawn behind the body: most of the (much taller) nose sprite ends up
    // hidden behind the opaque body art, leaving just the tip visible above
    // the head instead of a long stripe floating above or dangling below.
    if (shootingPose)
        window.draw(noseSprite);
    window.draw(sprite);
}

void Player::applyPoseScale() {
    sf::Vector2f s = shootingPose ? shootScale : normalScale;
    sprite.setScale({s.x * shrinkFactor, s.y * shrinkFactor});
    noseSprite.setScale({noseScale.x * shrinkFactor, noseScale.y * shrinkFactor});
}

void Player::setShootingInput(bool held) {
    if (held && !shootingPose)
        shootTimer = 1e6f; // fire immediately on the frame shooting starts

    if (held != shootingPose) {
        shootingPose = held;
        if (shootingPose)
            sprite.setTexture(shootingTexture, true);
        applyPoseScale();
    }
}

bool Player::updateShootTimer(float dt, float cooldown) {
    if (!shootingPose || shrinking)
        return false;

    shootTimer += dt;
    if (shootTimer >= cooldown) {
        shootTimer = 0.f;
        return true;
    }
    return false;
}

sf::Vector2f Player::getMuzzlePosition() const {
    sf::FloatRect noseBounds = noseSprite.getGlobalBounds();
    return { noseBounds.position.x + noseBounds.size.x / 2.f, noseBounds.position.y };
}

void Player::updateNosePosition() {
    sf::FloatRect b = sprite.getGlobalBounds();
    sf::FloatRect noseLocal = noseSprite.getLocalBounds();
    float noseW = noseLocal.size.x * noseScale.x * shrinkFactor;

    // The sprite's bounding box top isn't always where the head visually
    // starts - the shooting pose in particular has empty space above the
    // raised arm. Shift down by that texture's measured padding first to
    // find the real head top, then apply the same tip-rise as before.
    float headTopRatio = shootingPose ? shootHeadTopRatio
                                       : (facingLeft ? leftHeadTopRatio : rightHeadTopRatio);
    float headTopY = b.position.y + headTopRatio * b.size.y;

    // Only NOSE_TIP_RISE pixels poke above the head; the rest of the (much
    // taller) nose sprite extends down behind the body, where it is masked
    // by the opaque body art drawn on top of it.
    float x = b.position.x + b.size.x / 2.f - noseW / 2.f;
    float y = headTopY - Constants::NOSE_TIP_RISE * shrinkFactor;
    noseSprite.setPosition({x, y});
}

void Player::startShrinking(sf::Vector2f targetCenter) {
    if (shrinking || consumed)
        return;
    shrinking = true;
    shrinkTimer = 0.f;
    velocity = {0.f, 0.f};

    sf::FloatRect b = sprite.getGlobalBounds();
    shrinkStartCenter = { b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f };
    shrinkTarget = targetCenter;
}

void Player::updateShrink(float dt) {
    shrinkTimer += dt;
    float t = std::clamp(shrinkTimer / Constants::PLAYER_SHRINK_DURATION, 0.f, 1.f);
    shrinkFactor = 1.f - t;
    applyPoseScale();

    // Pulled in toward the hole's center as it shrinks, like being sucked in.
    sf::Vector2f center = shrinkStartCenter + (shrinkTarget - shrinkStartCenter) * t;
    sf::FloatRect b = sprite.getGlobalBounds(); // reflects the size after applyPoseScale
    sprite.setPosition({center.x - b.size.x / 2.f, center.y - b.size.y / 2.f});

    if (shrinkFactor <= 0.f)
        consumed = true;
}
