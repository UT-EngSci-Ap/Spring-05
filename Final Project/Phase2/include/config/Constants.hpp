#pragma once

#include <SFML/System/Vector2.hpp>

namespace Constants {
    const float WINDOW_WIDTH = 600.f;
    const float WINDOW_HEIGHT = 800.f;
    const float SCREEN_MIDDLE = 400.f;

    // Game difficulty. Selected on the Settings screen and persisted to disk.
    enum class Difficulty { Easy, Medium, Hard };

    // player
    const float PLAYER_WIDTH = 60.f;
    const float PLAYER_HEIGHT = 55.f;
    const sf::Vector2f PLAYER_INITIAL_POS(300.f, 200.f);


    const float JUMP_SPEED = -500.f;
    const float GRAVITY = 500.f;
    const float HORIZONTAL_SPEED = 250.f;

    // Bounce speed applied when the player lands cleanly on top of a monster
    // (noticeably stronger than a spring, per the "so much higher" spec).
    const float MONSTER_BOUNCE_SPEED = -1000.f;

    // platform
    const float PLATFORM_WIDTH = 80.f;
    const float PLATFORM_HEIGHT = 20.f;
    const int INITIAL_PLATFORM_COUNT = 20;

    // No two platforms may overlap or touch: every platform must keep at least
    // this clear gap (in pixels) from every other platform on all sides.
    // The same clearance is reused for monsters and holes so that no two
    // world objects (platform/monster/hole) ever overlap or touch either.
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

    // On Medium/Hard, platform gaps are sampled from the upper portion of the
    // safe range below, so platforms end up more spread out (fewer of them
    // over a given climb) while every gap still stays within
    // SAFE_MAX_PLATFORM_GAP - reachability is never compromised.
    const float REDUCED_DENSITY_GAP_BIAS = 0.5f;
    const float MOVING_PLATFORM_SPEED = 80.f;
    // Multiplier applied to moving-platform speed on Medium/Hard.
    const float MOVING_PLATFORM_SPEED_MULT_HARDER = 1.6f;

    // monsters
    const float MONSTER_WIDTH = 70.f;
    const float MONSTER_HEIGHT = 42.f;
    const float MONSTER_SPEED = 60.f;
    const float MONSTER_SPAWN_CHANCE = 0.22f;
    const int MONSTER_HEALTH_EASY = 3;
    const int MONSTER_HEALTH_HARDER = 5; // Medium & Hard

    // Neither monsters nor holes may spawn until the player has climbed at
    // least this far above the starting platform. This guarantees a short,
    // hazard-free opening stretch at the beginning of every session, so the
    // player can never be killed in the first second by something that
    // spawned right next to their starting position.
    const float SAFE_START_HEIGHT = 500.f;

    // bullets (shot from the player's nose while shooting)
    const float BULLET_RADIUS = 6.f;
    const float BULLET_SPEED = 900.f; // much faster than the player itself
    const float SHOOT_COOLDOWN_EASY = 0.25f;
    const float SHOOT_COOLDOWN_HARDER = 0.45f; // Medium & Hard: slower fire rate

    // shooting pose / nose composite
    // Nose.png is a tall, narrow snout (28x120 native) - much taller than the
    // player itself. It is drawn BEHIND the body sprite (see Player::render),
    // so most of its length is naturally hidden behind the opaque body art
    // and only a small tip pokes out above the head, looking attached rather
    // than floating or dangling past the feet.
    const float NOSE_WIDTH = 12.f;
    const float NOSE_HEIGHT = NOSE_WIDTH * (120.f / 28.f); // preserves Nose.png aspect ratio
    const float NOSE_TIP_RISE = 10.f; // how far the visible tip pokes above the head

    // holes (Hard mode only) — small and big variants
    const float HOLE_SMALL_SIZE = 55.f;
    const float HOLE_BIG_SIZE = 95.f;
    const float HOLE_SPAWN_CHANCE = 0.15f;
    // Time it takes for the player to fully shrink away once caught by a hole.
    const float PLAYER_SHRINK_DURATION = 1.1f;

    // settings persistence defaults
    const int DEFAULT_VOLUME = 70;
}
