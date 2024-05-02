#pragma once

#include "sim.h"
class Playground {
    public:
    Playground();

    void update(float dt);
    void draw();

    void handelMouseInput();
    void handelMouseMovement();

    //vars
    Sim simulation;
};