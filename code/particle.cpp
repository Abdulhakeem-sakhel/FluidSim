#include "particle.h"
#include <raylib.h>

Particle::Particle(Vector2 position, Vector2 prevPosition, Vector2 velocity, Color color): 
    position(position), 
    prevPosition(prevPosition),
    velocity(velocity),
    color(color) {
}

