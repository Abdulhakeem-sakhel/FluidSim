#pragma once
#include "fluidhashgrid.h"
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
    void applyGravity(float dt);
    void doubleDensityRelaxation(float dt);
    void neighbourSearch();
    void worldBoundary();

    
    long PARTICLE_NUMBERS;
    float VELOCITY_DAMPING;
    std::vector<Particle> particles;
    FluidHashGrid fluidHashGrid;
    
private:

    //consts
    static constexpr Vector2 GRAVITY = {0, 1};
    static constexpr float REST_DENSITY = 10;
    static constexpr float k_NEAR = 5;
    static constexpr float k = 0.15;
    static constexpr float INTERACTION_RADIUS = 25;

};