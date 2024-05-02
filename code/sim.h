#pragma once
#include "particle.h"
#include <raylib.h>
#include <vector>
class Sim {
public:
    Sim();

    void update(float dt);
    void draw();
    void setParticleGrid();
    void predictPosition(float dt);
    void computeNextVelocity(float dt);
private:
    long PARTICLE_NUMBERS;
    float VELOCITY_DAMPING;
    std::vector<Particle> particles;
};