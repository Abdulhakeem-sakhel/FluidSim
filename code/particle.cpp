#include "particle.h"
#include "utils.h"
#include <raylib.h>

Particle::Particle(Vector2 position): 
    position(position), 
    prevPosition(position),
    velocity(Vec2Ops::ZERO),
    color(SKYBLUE) {
}

