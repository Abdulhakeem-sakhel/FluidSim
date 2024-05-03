#include "playground.h"
#include "sim.h"
#include <iostream>
#include <raylib.h>

Playground::Playground() {
    simulation = Sim();
}

void Playground::update(float dt) {
    if (dt == 0) return;
    simulation.update(dt*100);
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
}
void Playground::handelMouseMovement() {
    Vector2 mouseDelta = GetMouseDelta();
    if(mouseDelta.x != 0 || mouseDelta.y != 0) {
        Vector2 mousePos = GetMousePosition();
        // prev pos currentPos - delta
    }

}