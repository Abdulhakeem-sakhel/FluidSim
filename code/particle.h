#pragma once

#include <raylib.h>
#include "utils.h"
class Particle {
    public:
    Particle(Vector2 position = Vec2Ops::ZERO);

    Vector2 position;
    Vector2 prevPosition;
    Vector2 velocity;
    Color color;
};