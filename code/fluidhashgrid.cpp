#include "fluidhashgrid.h"
#include "particle.h"
#include <cmath>
#include <cstdint>
#include <functional>
#include <raylib.h>
#include <vector>

FluidHashGrid::FluidHashGrid(float cellSize, std::vector<Particle> &particles)
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

std::vector<uint32_t> FluidHashGrid::getNeighbourOfParticleIdx(uint64_t i) {
    auto neighbors = std::vector<uint32_t>();
    
    uint64_t particleGridX= static_cast<uint64_t>(particles[i].position.x / cellSize);
    uint64_t particleGridY= static_cast<uint64_t>(particles[i].position.y / cellSize);

    for (int x = -1; x <= 1; x++) {
        for(int y = -1; y<=1; y++) {
            uint64_t gridX = particleGridX + x;
            uint64_t gridY = particleGridY + y;

            uint64_t hash = cellIndexToHash(gridX, gridY);
            auto content = getContentCell(hash);

            if (content == nullptr) continue;
            neighbors.insert(neighbors.end(), content->begin(), content->end());
        }
    }

    return neighbors;
}

void FluidHashGrid::mapParticleToCell() {

    for (int i = 0; i < particles.size(); i++) {
        uint64_t hash =  getGridHashFromPosition(particles[i].position);
        auto it = map.find(hash);

        if (it == map.end()) {
            auto particleGrid = std::vector<uint32_t>();
            particleGrid.push_back(i);
            //map.emplace(hash, particleGrid); // Use emplace to construct the value in-place
            map[hash] = particleGrid;
        } else {
            it->second.push_back(i);
        }
    }
}

std::vector<uint32_t> *FluidHashGrid::getContentCell(uint64_t id) {
    auto it = map.find(id);

    if (it == map.end()) {
        return nullptr;
    }

    return &it->second;
}

void FluidHashGrid::clearGrid() {
    map.clear();
}