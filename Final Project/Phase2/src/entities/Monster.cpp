#include "entities/Monster.hpp"
#include "entities/Player.hpp"
#include "config/Constants.hpp"
#include "utils/Utils.hpp"

Monster::Monster(float x, float y, int healthIn, float speedIn, bool blueVariant)
    : Entity(x, y), health(healthIn), speed(speedIn)
{
    Utils::loadTexture(texture, blueVariant ? "assets/images/BlueMonster.png"
                                             : "assets/images/green_monster.png");
    sprite.setTexture(texture, true);
    float scaleX = Constants::MONSTER_WIDTH / sprite.getLocalBounds().size.x;
    float scaleY = Constants::MONSTER_HEIGHT / sprite.getLocalBounds().size.y;
    sprite.setScale({scaleX, scaleY});
}

void Monster::update(float dt) {
    if (dead)
        return;

    sprite.move({speed * dt, 0.f});
    float x = sprite.getPosition().x;
    if (x < 0.f || x > Constants::WINDOW_WIDTH - Constants::MONSTER_WIDTH)
        speed = -speed;
}

void Monster::move(float x, float y) {
    sprite.move({x, y});
}

void Monster::reset(float x, float y, int healthIn, float speedIn) {
    health = healthIn;
    speed = speedIn;
    dead = false;
    sprite.setPosition({x, y});
}

Monster::CollisionResult Monster::resolvePlayerCollision(Player& player) {
    if (dead)
        return CollisionResult::None;

    sf::FloatRect playerBounds = player.getGlobalBounds();
    sf::FloatRect monsterBounds = getGlobalBounds();
    if (!playerBounds.findIntersection(monsterBounds).has_value())
        return CollisionResult::None;

    float prevPlayerBottom = player.getPrivousPos().y + playerBounds.size.y;
    float monsterTop = monsterBounds.position.y;

    // Falling and landing cleanly on top: snap the player onto the
    // monster's surface (like landing on a platform) before bouncing it,
    // otherwise the player stays embedded in the monster's box for a frame
    // or two after the bounce, and the next check - now seeing an upward
    // velocity while still overlapping - would wrongly fall through to Kill.
    if (player.getVelY() > 0.f && prevPlayerBottom <= monsterTop) {
        player.setPosition(playerBounds.position.x, monsterTop - playerBounds.size.y);
        player.monsterBounceJump();
        return CollisionResult::Bounce;
    }

    // Any other overlap (from below or the side) is fatal.
    return CollisionResult::Kill;
}

void Monster::takeHit() {
    if (dead)
        return;
    health--;
    if (health <= 0)
        dead = true;
}
