#pragma once

#include <SFML/System/Vector2.hpp>

namespace Constants {
    const float WINDOW_WIDTH = 600.f;
    const float WINDOW_HEIGHT = 800.f;
    const float SCREEN_MIDDLE = 400.f;

    // player
    const float PLAYER_WIDTH = 60.f;
    const float PLAYER_HEIGHT = 55.f;
    const sf::Vector2f PLAYER_INITIAL_POS(300.f, 200.f);


    const float JUMP_SPEED = -500.f;
    const float GRAVITY = 500.f;
    const float HORIZONTAL_SPEED = 250.f;

    // platform
    const float PLATFORM_WIDTH = 80.f;
    const float PLATFORM_HEIGHT = 20.f;
    const int INITIAL_PLATFORM_COUNT = 20;

    // No two platforms may overlap or touch: every platform must keep at least
    // this clear gap (in pixels) from every other platform on all sides.
    const float MIN_PLATFORM_DISTANCE = 25.f;
    const int PLATFORM_PLACEMENT_ATTEMPTS = 40;

    // spring on platform
    const float SPRING_WIDTH = 30.f;
    const float SPRING_HEIGHT = 30.f;

    // physics formula for maximum jump calculated off of jump speed and gravity
    const float MAX_PLATFORM_GAP = (JUMP_SPEED * JUMP_SPEED) / (2 * GRAVITY);
    const float MIN_PLATFORM_GAP = 45.f;
    // physics formula for maximum air time when jumping off of a platform
    const float MAX_AIR_TIME = 2 * (-JUMP_SPEED / GRAVITY);
    // maximum horizontal distance reachable during a single jump
    const float MAX_HORIZONTAL_MOVE = HORIZONTAL_SPEED * MAX_AIR_TIME;

    const int NUM_OF_PLATFORMS_TYPE = 3;

    // Reachability guarantees — there must ALWAYS be a way to climb higher.
    // Cap the vertical gap safely below the theoretical jump apex so the player
    // always overshoots a target platform and can land on it while descending.
    const float SAFE_MAX_PLATFORM_GAP = 0.80f * MAX_PLATFORM_GAP;
    // Fraction of the physically-reachable horizontal distance we actually use,
    // leaving a safety margin for imperfect timing and platform width.
    const float HORIZONTAL_REACH_SAFETY = 0.85f;
}
