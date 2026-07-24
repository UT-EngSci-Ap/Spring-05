#include "systems/CollisionSystem.hpp"

void CollisionSystem::resolvePlayerPlatforms(
    Player& player,
    std::vector<Platform*>& platforms)
{
    sf::FloatRect playerBounds = player.getGlobalBounds();

    for (auto* platform : platforms) {
        sf::FloatRect platBounds = platform->getBounds();

        if (playerBounds.findIntersection(platBounds).has_value()) {
            float prevPlayerBottom = player.getPrivousPos().y + playerBounds.size.y;
            float platTop = platBounds.position.y;

            if (player.getVelY() > 0 && prevPlayerBottom <= platTop) {
                player.setPosition(playerBounds.position.x, platTop - playerBounds.size.y);
                platform->onLandPlayer(player);
            }
        }
    }
}

void CollisionSystem::resolvePlayerSpring(
    Player& player,
    std::vector<Platform*>& platforms
)
{
    for (Platform* plat : platforms) {
        plat->handleSpringCollision(player);
    }
}
