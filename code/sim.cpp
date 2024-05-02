#include "sim.h"
#include "particle.h"
#include <cmath>
#include <raylib.h>
#include <vector>

const int PARTICLE_RADIUS = 20;


Sim::Sim(): PARTICLE_NUMBERS(100), VELOCITY_DAMPING(1) {
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    particles = std::vector<Particle>(PARTICLE_NUMBERS);
    int row = std::sqrt(PARTICLE_NUMBERS);;
    int column = std::sqrt(PARTICLE_NUMBERS);

    Vector2 leftCorner = Vector2 {
        (width / 2.f) - (column / 2.f * PARTICLE_RADIUS),
        (height / 2.f) - (row / 2.f * PARTICLE_RADIUS)
    };
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            float xPosition = leftCorner.x + (j*(PARTICLE_RADIUS*2));
            float yPosition = leftCorner.y + (i*(PARTICLE_RADIUS*2));
            particles[i*column + j].position = Vector2{xPosition, yPosition};
        }
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