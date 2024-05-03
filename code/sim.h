#pragma once
#include "particle.h"
#include <raylib.h>
#include <vector>
class Sim {
public:
    Sim();

    void update(float dt);
    void draw();
    void initPartialInGrid(int offset);
    void predictPosition(float dt);
    void computeNextVelocity(float dt);
    void worldBoundary();
private:
    long PARTICLE_NUMBERS;
    float VELOCITY_DAMPING;
    std::vector<Particle> particles;
};