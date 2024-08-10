#include "sim.h"
#include "fluidhashgrid.h"
#include "particle.h"
#include "utils.h"
#include <cmath>
#include <cstdlib>
#include <raylib.h>
#include <vector>

using myMaths::randf;

const int PARTICLE_RADIUS = 5;

Sim::Sim(): PARTICLE_NUMBERS(2500), VELOCITY_DAMPING(1),
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
    for (int i = 0; i < particles.size(); i++) {
        float density = 0.f;
        float nearDensity = 0.f;
        auto neighbourParticles = fluidHashGrid.getNeighbourOfParticleIdx(i);
        for (int j = 0; j < neighbourParticles.size(); j++) {
            if (&particles[neighbourParticles[j]] == &particles[i]) continue;

            Vector2 rij = Vec2Ops::sub(particles[neighbourParticles[j]].position, particles[i].position);
            float q = Vec2Ops::length(rij) / INTERACTION_RADIUS;

            if (q < 1.f) {
                density += std::pow(1 - q, 2);
                nearDensity += std::pow(1 - q, 3);

            }
        }

        auto pressure = k * (density - REST_DENSITY);
        auto pressureNear = k_NEAR * nearDensity;
        Vector2 particleADisplacement = Vec2Ops::ZERO;

        for (int j = 0; j < neighbourParticles.size(); j++) {
            if (&particles[neighbourParticles[j]] == &particles[i]) continue;

            Vector2 rij = Vec2Ops::sub(particles[neighbourParticles[j]].position, particles[i].position);
            float q = Vec2Ops::length(rij) / INTERACTION_RADIUS;

            if (q < 1.f) {
                Vec2Ops::normalize(rij);
                auto displacementTerm = std::pow(dt, 2) *
                    (pressure * (1-q) + pressureNear * std::pow(1-q, 2));

                Vector2 D = Vec2Ops::scale(rij, displacementTerm);
                particles[neighbourParticles[j]].position = Vec2Ops::add(particles[neighbourParticles[j]].position,
                    Vec2Ops::scale(D, .5));
                particleADisplacement = Vec2Ops::sub(particleADisplacement, 
                    Vec2Ops::scale(D, .5));
                
            }
        }
        particles[i].position = Vec2Ops::add(particles[i].position, particleADisplacement);
    }
}

void Sim::neighbourSearch() {
    fluidHashGrid.clearGrid();
    fluidHashGrid.mapParticleToCell();
}

void Sim::worldBoundary() {
    for (auto &particle: particles) {


        if (particle.position.x < PARTICLE_RADIUS) {
            // particle.velocity.x *= -1;
            particle.position.x = PARTICLE_RADIUS;
            particle.prevPosition.x = PARTICLE_RADIUS;
        }

        if (particle.position.y < PARTICLE_RADIUS) {
            // particle.velocity.y *= -1;
            particle.position.y = PARTICLE_RADIUS;
            particle.prevPosition.y = PARTICLE_RADIUS;
        }

        if (particle.position.x > GetScreenWidth() - PARTICLE_RADIUS ) {
            // particle.velocity.x *= -1 ;
            particle.position.x = GetScreenWidth() - PARTICLE_RADIUS - 1;
            particle.prevPosition.x = GetScreenWidth() - PARTICLE_RADIUS - 1;
        }

        if (particle.position.y > GetScreenHeight() - PARTICLE_RADIUS ) {
            // particle.velocity.y *= -1;
            particle.position.y = GetScreenHeight() - PARTICLE_RADIUS - 1;
            particle.prevPosition.y = GetScreenHeight() - PARTICLE_RADIUS - 1;
        }
    }
}

void Sim::update(float dt) {
    applyGravity(dt);
    predictPosition(dt);
    neighbourSearch();
    doubleDensityRelaxation(dt);
    worldBoundary();
    computeNextVelocity(dt);
}

void Sim::draw() {
    for (const auto& particle: particles) {
        DrawCircle(particle.position.x, particle.position.y, PARTICLE_RADIUS, particle.color);
    }
}