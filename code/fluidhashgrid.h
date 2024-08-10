#pragma once
#include "particle.h"
#include <cstddef>
#include <cstdint>
#include <raylib.h>
#include <unordered_map>
#include <vector>

class FluidHashGrid {
    public:
    FluidHashGrid(float cellSize, std::vector<Particle> &particles);

    uint64_t getGridHashFromPosition(Vector2 position);
    uint64_t cellIndexToHash(uint64_t x, uint64_t y);
    std::vector<uint32_t> getNeighbourOfParticleIdx(uint64_t i);
    void mapParticleToCell();
    void clearGrid();
    std::vector<uint32_t> *getContentCell(uint64_t id);

    float cellSize;
    std::unordered_map<uint64_t, std::vector<uint32_t>> map;
    uint64_t  hashMapSize;
    std::vector<uint32_t> particles_index;
    std::vector<Particle> &particles;
};
