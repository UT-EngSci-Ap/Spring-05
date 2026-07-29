#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include "platforms/Platform.hpp"
#include "entities/Player.hpp"
#include "entities/Monster.hpp"
#include "entities/Bullet.hpp"
#include "entities/Hole.hpp"
#include <vector>

class CollisionSystem {
public:
    // Returns true if the player bounced off a non-breakable platform this
    // frame (used by the caller to trigger the jump sound).
    static bool resolvePlayerPlatforms(
        Player& player,
        std::vector<Platform*>& platforms);

    static void resolvePlayerSpring(
        Player& player,
        std::vector<Platform*>& platforms
    );

    // Returns Bounce/Kill/None (see Monster::CollisionResult).
    static Monster::CollisionResult resolvePlayerMonsters(
        Player& player,
        std::vector<Monster*>& monsters);

    // Bullets that hit a monster are marked dead and the monster takes a hit.
    static void resolveBulletsMonsters(
        std::vector<Bullet*>& bullets,
        std::vector<Monster*>& monsters);

    // Returns true the moment the player touches a hole (caller starts the
    // shrink effect).
    static bool resolvePlayerHoles(
        Player& player,
        std::vector<Hole*>& holes);

};

#endif
