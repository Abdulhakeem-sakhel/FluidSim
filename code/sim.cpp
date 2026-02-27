#include "sim.h"
#include "fluidhashgrid.h"
#include "particle.h"
#include "profiler.h"
#include "utils.h"
#include <cmath>
#include <cstdlib>
#include <raylib.h>
#include <vector>

using myMaths::randf;

const int PARTICLE_RADIUS = 5;

Sim::Sim(): PARTICLE_NUMBERS(5000), VELOCITY_DAMPING(1),
    fluidHashGrid(INTERACTION_RADIUS, particles){

    //let test it out after and before
    particles.resize(PARTICLE_NUMBERS);
    initPartialInGrid(3);
}

void Sim::initPartialInGrid(int offset) {
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    int row = std::sqrt(PARTICLE_NUMBERS);;
    int column = std::sqrt(PARTICLE_NUMBERS);

    Vector2 leftCorner = Vector2 {
        (width / 2.f) - (column  * PARTICLE_RADIUS),
        (height / 2.f) - (row  * PARTICLE_RADIUS)
    };
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            float xPosition = leftCorner.x + (j*(PARTICLE_RADIUS*2 + offset));
            float yPosition = leftCorner.y + (i*(PARTICLE_RADIUS*2 + offset));
            particles[i*column + j].position = Vector2{xPosition, yPosition};
            particles[i*column + j].prevPosition = Vector2{xPosition, yPosition};
            particles[i * column + j].velocity =
                Vector2{myMaths::randf() - .5f, randf() - .5f};
        }
    }
    
    if (row * column == PARTICLE_NUMBERS) return;
    int leftParticle = PARTICLE_NUMBERS - (row*column);
    for(int i =0; i < leftParticle; i++) {
        float xPosition = leftCorner.x + ((i % column)*(PARTICLE_RADIUS*2 + offset));
        if (i % column == 0 && i != 0) 
            row++;
        float yPosition = leftCorner.y + ((row)*(PARTICLE_RADIUS*2 + offset));
        particles[row*column + (i % column)].position = Vector2{xPosition, yPosition};
        particles[row*column + (i % column)].prevPosition = Vector2{xPosition, yPosition};
        particles[row*column + (i % column)].velocity = Vector2{randf() - .5f, randf() - .5f};
    }

}

void Sim::predictPosition(float dt) {
    for (auto &particle: particles) {
        particle.prevPosition = particle.position;
        Vector2 positionDelta = Vec2Ops::scale(particle.velocity, dt * VELOCITY_DAMPING);
        particle.position = Vec2Ops::add(particle.position, positionDelta);
    }
}

void Sim::computeNextVelocity(float dt) {
    for (auto &particle: particles) {
        Vector2 velocity = Vec2Ops::scale(
            Vec2Ops::sub(particle.position, particle.prevPosition),
            1.0 / dt);
        particle.velocity = velocity;
    }
}

void Sim::applyGravity(float dt) {
    for (auto &particle: particles) {
        particle.velocity = Vec2Ops::add(
            particle.velocity,
            Vec2Ops::scale(GRAVITY, dt)); 
    }
}

void Sim::doubleDensityRelaxation(float dt) {
    // Major hot-path optimizations:
    // - no per-particle neighbor allocations (callback iteration)
    // - no std::pow() in the inner loops
    // - early reject by squared-distance (avoid sqrt)
    // NOTE: This function updates neighbor particle positions, so a naive OpenMP parallel-for
    // would introduce data races and nondeterministic results.
    const float R = INTERACTION_RADIUS;
    const float invR = 1.0f / R;
    const float R2 = R * R;
    const float dt2 = dt * dt;

    for (uint32_t i = 0; i < static_cast<uint32_t>(particles.size()); ++i) {
        const Vector2 pi = particles[i].position;
        float density = 0.0f;
        float nearDensity = 0.0f;

        fluidHashGrid.forEachNeighbourOfParticleIdx(i, [&](uint32_t j) {
            if (j == i) return;
            const Vector2 pj = particles[j].position;
            const float dx = pj.x - pi.x;
            const float dy = pj.y - pi.y;
            const float r2 = dx * dx + dy * dy;
            if (r2 >= R2 || r2 <= 0.0f) return;

            const float q = std::sqrt(r2) * invR;
            const float oneMinus = 1.0f - q;
            const float oneMinus2 = oneMinus * oneMinus;
            density += oneMinus2;
            nearDensity += oneMinus2 * oneMinus;
        });

        const float pressure = k * (density - REST_DENSITY);
        const float pressureNear = k_NEAR * nearDensity;

        Vector2 displacementI = Vec2Ops::ZERO;

        fluidHashGrid.forEachNeighbourOfParticleIdx(i, [&](uint32_t j) {
            if (j == i) return;
            Vector2 &pj = particles[j].position;

            const float dx = pj.x - pi.x;
            const float dy = pj.y - pi.y;
            const float r2 = dx * dx + dy * dy;
            if (r2 >= R2 || r2 <= 1e-12f) return;

            const float invLen = 1.0f / std::sqrt(r2);
            const float q = (1.0f / invLen) * invR; // sqrt(r2) * invR
            const float oneMinus = 1.0f - q;
            const float oneMinus2 = oneMinus * oneMinus;

            const float displacementTerm = dt2 * (pressure * oneMinus + pressureNear * oneMinus2);
            const float nx = dx * invLen;
            const float ny = dy * invLen;

            const float Dx = nx * displacementTerm;
            const float Dy = ny * displacementTerm;

            pj.x += Dx * 0.5f;
            pj.y += Dy * 0.5f;
            displacementI.x -= Dx * 0.5f;
            displacementI.y -= Dy * 0.5f;
        });

        particles[i].position.x += displacementI.x;
        particles[i].position.y += displacementI.y;
    }
}

void Sim::neighbourSearch() {
    fluidHashGrid.setWorldSize(GetScreenWidth(), GetScreenHeight());
    fluidHashGrid.clearGrid();
    fluidHashGrid.mapParticleToCell();
}

void Sim::worldBoundary() {
    const float minX = static_cast<float>(PARTICLE_RADIUS);
    const float minY = static_cast<float>(PARTICLE_RADIUS);
    const float maxX = static_cast<float>(GetScreenWidth() - PARTICLE_RADIUS - 1);
    const float maxY = static_cast<float>(GetScreenHeight() - PARTICLE_RADIUS - 1);

    for (auto &particle: particles) {


        if (particle.position.x < minX) {
            // particle.velocity.x *= -1;
            particle.position.x = minX;
            particle.prevPosition.x = minX;
        }

        if (particle.position.y < minY) {
            // particle.velocity.y *= -1;
            particle.position.y = minY;
            particle.prevPosition.y = minY;
        }

        if (particle.position.x > maxX) {
            // particle.velocity.x *= -1 ;
            particle.position.x = maxX;
            particle.prevPosition.x = maxX;
        }

        if (particle.position.y > maxY) {
            // particle.velocity.y *= -1;
            particle.position.y = maxY;
            particle.prevPosition.y = maxY;
        }
    }
}

void Sim::update(float dt) {
    { FluidSimProfiler::Scope _s(FluidSimProfiler::Section::NeighbourSearch); neighbourSearch(); }
    { FluidSimProfiler::Scope _s(FluidSimProfiler::Section::Gravity); applyGravity(dt); }
    { FluidSimProfiler::Scope _s(FluidSimProfiler::Section::Predict); predictPosition(dt); }
    { FluidSimProfiler::Scope _s(FluidSimProfiler::Section::Relaxation); doubleDensityRelaxation(dt); }
    { FluidSimProfiler::Scope _s(FluidSimProfiler::Section::Boundary); worldBoundary(); }
    { FluidSimProfiler::Scope _s(FluidSimProfiler::Section::Velocity); computeNextVelocity(dt); }

    FluidSimProfiler::endFrame();
}

void Sim::draw() {
    for (const auto& particle: particles) {
        DrawCircle(particle.position.x, particle.position.y, PARTICLE_RADIUS, particle.color);
    }
}
