#include "playground.h"
#include "particle.h"
#include "sim.h"
#include "utils.h"
#include <iostream>
#include <raylib.h>

Playground::Playground(): simulation() {;
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
    simulation.particles.push_back(Particle{
        mousePos,
        mousePos,
        Vector2{myMaths::randf() -0.5f, myMaths::randf() -0.5f},
        BLUE,
    });
    simulation.PARTICLE_NUMBERS +=1;
}