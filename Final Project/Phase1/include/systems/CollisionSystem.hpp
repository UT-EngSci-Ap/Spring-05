#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include "platforms/Platform.hpp"
#include "entities/Player.hpp"
#include <vector>

class CollisionSystem {
public:
    static void resolvePlayerPlatforms(
        Player& player,
        std::vector<Platform*>& platforms);

    static void resolvePlayerSpring(
        Player& player,
        std::vector<Platform*>& platforms
    );

};

#endif
