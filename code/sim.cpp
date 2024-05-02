#include "sim.h"
#include "particle.h"
#include <cmath>
#include <raylib.h>
#include <vector>

const int PARTICLE_RADIUS = 5;


Sim::Sim(): PARTICLE_NUMBERS(1000), VELOCITY_DAMPING(1) {
    particles = std::vector<Particle>(PARTICLE_NUMBERS);
    setParticleGrid();
}

void Sim::setParticleGrid() {
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
            float xPosition = leftCorner.x + (j*(PARTICLE_RADIUS*2));
            float yPosition = leftCorner.y + (i*(PARTICLE_RADIUS*2));
            particles[i*column + j].position = Vector2{xPosition, yPosition};
            particles[i*column + j].prevPosition = Vector2{xPosition, yPosition};
        }
    }
    
    if (row * column == PARTICLE_NUMBERS) return;
    int leftParticle = PARTICLE_NUMBERS - (row*column);
    for(int i =0; i < leftParticle; i++) {
        float xPosition = leftCorner.x + ((i % column)*(PARTICLE_RADIUS*2));
        if (i % column == 0 && i != 0) 
            row++;
        float yPosition = leftCorner.y + ((row)*(PARTICLE_RADIUS*2));
        particles[row*column + (i % column)].position = Vector2{xPosition, yPosition};
        particles[row*column + (i % column)].prevPosition = Vector2{xPosition, yPosition};
    }
}

void Sim::update(float dt) {
    
}

void Sim::draw() {
    for (const auto& particle: particles) {
        DrawCircle(particle.position.x, particle.position.y, PARTICLE_RADIUS, particle.color);

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
        particle.velocity = Vec2Ops::scale(
            Vec2Ops::sub(particle.position, particle.prevPosition),
            1.0 / dt);
    }
}