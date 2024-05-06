#pragma once
#include "particle.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <raylib.h>
#include <unordered_map>
#include <vector>

class FluidHashGrid {
    public:
    FluidHashGrid(float cellSize, std::vector<std::shared_ptr<Particle>> particles);

    uint64_t getGridHashFromPosition(Vector2 position);
    uint64_t cellIndexToHash(uint64_t x, uint64_t y);
    std::vector<std::shared_ptr<Particle>> getNeighbourOfParticleIdx(uint64_t i);
    void mapParticleToCell();
    void clearGrid();
    std::vector<std::shared_ptr<Particle>> *getContentCell(uint64_t id);

    float cellSize;
    std::unordered_map<uint64_t, std::vector<std::shared_ptr<Particle>>> map;
    uint64_t  hashMapSize;
    std::vector<std::shared_ptr<Particle>> particles;
};
