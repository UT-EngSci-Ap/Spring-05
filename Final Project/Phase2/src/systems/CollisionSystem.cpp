#include "systems/CollisionSystem.hpp"

bool CollisionSystem::resolvePlayerPlatforms(
    Player& player,
    std::vector<Platform*>& platforms)
{
    // While being swallowed by a hole, the player is frozen and must not
    // interact with anything else.
    if (player.isShrinking() || player.isConsumed())
        return false;

    bool bounced = false;
    sf::FloatRect playerBounds = player.getGlobalBounds();

    for (auto* platform : platforms) {
        sf::FloatRect platBounds = platform->getBounds();

        if (playerBounds.findIntersection(platBounds).has_value()) {
            float prevPlayerBottom = player.getPrivousPos().y + playerBounds.size.y;
            float platTop = platBounds.position.y;

            if (player.getVelY() > 0 && prevPlayerBottom <= platTop) {
                player.setPosition(playerBounds.position.x, platTop - playerBounds.size.y);
                if (platform->onLandPlayer(player))
                    bounced = true;
            }
        }
    }
    return bounced;
}

void CollisionSystem::resolvePlayerSpring(
    Player& player,
    std::vector<Platform*>& platforms
)
{
    // While being swallowed by a hole, the player is frozen and must not
    // interact with anything else.
    if (player.isShrinking() || player.isConsumed())
        return;

    for (Platform* plat : platforms) {
        plat->handleSpringCollision(player);
    }
}

Monster::CollisionResult CollisionSystem::resolvePlayerMonsters(
    Player& player,
    std::vector<Monster*>& monsters)
{
    // While being swallowed by a hole, the player is frozen and must not
    // interact with anything else - in particular, a monster touched while
    // motionless (velocity.y == 0) would otherwise fall through to Kill.
    if (player.isShrinking() || player.isConsumed())
        return Monster::CollisionResult::None;

    for (Monster* monster : monsters) {
        Monster::CollisionResult result = monster->resolvePlayerCollision(player);
        if (result != Monster::CollisionResult::None)
            return result;
    }
    return Monster::CollisionResult::None;
}

void CollisionSystem::resolveBulletsMonsters(
    std::vector<Bullet*>& bullets,
    std::vector<Monster*>& monsters)
{
    for (Bullet* bullet : bullets) {
        if (bullet->isDead())
            continue;
        for (Monster* monster : monsters) {
            if (monster->isDead())
                continue;
            if (bullet->getBounds().findIntersection(monster->getGlobalBounds()).has_value()) {
                bullet->kill();
                monster->takeHit();
                break;
            }
        }
    }
}

bool CollisionSystem::resolvePlayerHoles(
    Player& player,
    std::vector<Hole*>& holes)
{
    if (player.isShrinking() || player.isConsumed())
        return false;

    sf::FloatRect playerBounds = player.getGlobalBounds();
    for (Hole* hole : holes) {
        sf::FloatRect holeBounds = hole->getGlobalBounds();
        if (playerBounds.findIntersection(holeBounds).has_value()) {
            sf::Vector2f center(holeBounds.position.x + holeBounds.size.x / 2.f,
                                holeBounds.position.y + holeBounds.size.y / 2.f);
            player.startShrinking(center);
            return true;
        }
    }
    return false;
}
