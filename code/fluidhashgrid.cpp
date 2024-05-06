#include "fluidhashgrid.h"
#include "particle.h"
#include <cmath>
#include <cstdint>
#include <functional>
#include <memory>
#include <raylib.h>
#include <vector>

FluidHashGrid::FluidHashGrid(float cellSize, std::vector<std::shared_ptr<Particle>> particles)
    : cellSize(cellSize),
    hashMapSize(1000000),
    particles(particles) {
}

uint64_t FluidHashGrid::getGridHashFromPosition(Vector2 position) {
    uint64_t x= static_cast<uint64_t>(position.x / cellSize);
    uint64_t y= static_cast<uint64_t>(position.y / cellSize);

    return cellIndexToHash(x, y);
}

uint64_t FluidHashGrid::cellIndexToHash(uint64_t x, uint64_t y) {
    uint64_t combined = (x << 32) | y;
    
    std::hash<uint64_t> hasher;
    uint64_t hash = hasher(combined);

    hash %= hashMapSize;

    return hash;
}

void FluidHashGrid::mapParticleToCell() {

    for (auto particle: particles) {
        uint64_t hash =  getGridHashFromPosition(particle->position);

        auto it = map.find(hash);
        if (it == map.end()) {
            auto particleGrid = std::vector<std::shared_ptr<Particle>>();
            particleGrid.push_back(particle);
            //map.emplace(hash, particleGrid); // Use emplace to construct the value in-place
            map[hash] = particleGrid;
        } else {
            it->second.push_back(particle);
        }
    }
}

std::vector<std::shared_ptr<Particle>> *FluidHashGrid::getContentCell(uint64_t id) {
    auto it = map.find(id);

    if (it == map.end()) {
        return nullptr;
    }

    return &it->second;
}

void FluidHashGrid::clearGrid() {
    map.clear();
}