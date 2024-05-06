#pragma once

#include "sim.h"
class Playground {
    public:
    Playground();

    void update(float dt);
    void draw();

    void handelMouseInput();

    void spawnParticle(Vector2 mousePos);    
    //vars
    Sim simulation;
};