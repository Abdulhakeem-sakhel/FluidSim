#include "playground.h"
#include "particle.h"
#include "sim.h"
#include "utils.h"
#include <iostream>
#include <memory>
#include <raylib.h>

Playground::Playground() {
    simulation = Sim();
}

void Playground::update(float dt) {
    if (dt == 0) return;
    simulation.update(dt*20);
}

void Playground::draw() {
    simulation.draw();
}


void Playground::handelMouseInput() {
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        std::cout << "Mouse Down\n";
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        std::cout << "Mouse pressed\n";
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        std::cout << "spawned particle\n";
        spawnParticle(GetMousePosition());
    }
}

void Playground::spawnParticle(Vector2 mousePos){
    std::shared_ptr<Particle> particle = std::make_shared<Particle>();
    particle->position = mousePos;
    particle->prevPosition = mousePos;
    particle->velocity = Vector2{myMaths::randf() -0.5f, myMaths::randf() -0.5f};
    particle->color = BLUE;

    simulation.PARTICLE_NUMBERS +=1;
    simulation.particles.push_back(particle);
    simulation.fluidHashGrid.particles.push_back(particle);
}