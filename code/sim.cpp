#include "sim.h"
#include "fluidhashgrid.h"
#include "particle.h"
#include "utils.h"
#include <cmath>
#include <cstdlib>
#include <memory>
#include <raylib.h>
#include <vector>

const int PARTICLE_RADIUS = 5;


Sim::Sim(): PARTICLE_NUMBERS(1000), VELOCITY_DAMPING(1),
    fluidHashGrid(25, particles){
    for(int i = 0; i < PARTICLE_NUMBERS; i++) {  
        particles.push_back(std::make_shared<Particle>());
    }

    //let test it out after and before
    initPartialInGrid(3);
    fluidHashGrid.particles = particles;
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
            particles[i*column + j]->position = Vector2{xPosition, yPosition};
            particles[i*column + j]->prevPosition = Vector2{xPosition, yPosition};
            particles[i*column + j]->velocity = Vector2{myMaths::randf() - .5f, myMaths::randf() - .5f};
        }
    }
    
    if (row * column == PARTICLE_NUMBERS) return;
    int leftParticle = PARTICLE_NUMBERS - (row*column);
    for(int i =0; i < leftParticle; i++) {
        float xPosition = leftCorner.x + ((i % column)*(PARTICLE_RADIUS*2 + offset));
        if (i % column == 0 && i != 0) 
            row++;
        float yPosition = leftCorner.y + ((row)*(PARTICLE_RADIUS*2 + offset));
        particles[row*column + (i % column)]->position = Vector2{xPosition, yPosition};
        particles[row*column + (i % column)]->prevPosition = Vector2{xPosition, yPosition};
        particles[row*column + (i % column)]->velocity = Vector2{myMaths::randf() - .5f, myMaths::randf() - .5f};
    }

}

void Sim::predictPosition(float dt) {
    for (auto &particle: particles) {
        particle->prevPosition = particle->position;
        Vector2 positionDelta = Vec2Ops::scale(particle->velocity, dt * VELOCITY_DAMPING);
        particle->position = Vec2Ops::add(particle->position, positionDelta);
    }
}

void Sim::computeNextVelocity(float dt) {
    for (auto &particle: particles) {
        Vector2 velocity = Vec2Ops::scale(
            Vec2Ops::sub(particle->position, particle->prevPosition),
            1.0 / dt);
        particle->velocity = velocity;
    }
}

void Sim::neighbourSearch(Vector2 mousePosition) {
    fluidHashGrid.clearGrid();
    fluidHashGrid.mapParticleToCell();

    particles[0]->position = mousePosition;
    auto content = fluidHashGrid.getNeighbourOfParticleIdx(0);

    for(auto particle: particles) {
        particle->color = BLUE;
    }

    for(auto particle: content) { 
        Vector2 direction = Vec2Ops::sub(particle->position, mousePosition);
        float lengthSquared = Vec2Ops::length2(direction);

        if (lengthSquared <= fluidHashGrid.cellSize * fluidHashGrid.cellSize) 
            particle->color = YELLOW;
    }
}

void Sim::worldBoundary() {
    for (auto &particle: particles) {
        if (particle->position.x < PARTICLE_RADIUS) {
            particle->velocity.x *= -1;
        }

        if (particle->position.y < PARTICLE_RADIUS) {
            particle->velocity.y *= -1;
        }

        if (particle->position.x > GetScreenWidth() - PARTICLE_RADIUS ) {
            particle->velocity.x *= -1;
        }

        if (particle->position.y > GetScreenHeight() - PARTICLE_RADIUS ) {
            particle->velocity.y *= -1;
        }
    }
}

void Sim::update(float dt) {
    neighbourSearch(GetMousePosition());
    //predictPosition(dt);
    //computeNextVelocity(dt);
    worldBoundary();
}

void Sim::draw() {
    for (const auto& particle: particles) {
        DrawCircle(particle->position.x, particle->position.y, PARTICLE_RADIUS, particle->color);
    }
}