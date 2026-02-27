#pragma once
#include "particle.h"
#include <cstddef>
#include <cstdint>
#include <raylib.h>
#include <vector>

class FluidHashGrid {
    public:
    FluidHashGrid(float cellSize, std::vector<Particle> &particles);

    // Must be called when the simulation domain changes (window resize, etc.).
    void setWorldSize(int width, int height);

    // Build cell -> particle linked lists for current particle positions.
    void clearGrid();
    void mapParticleToCell();

    // Iterate over all particles found in the 3x3 neighboring cells around particle i.
    // This avoids per-particle allocations (unlike returning a std::vector).
    template <class Fn>
    inline void forEachNeighbourOfParticleIdx(uint32_t i, Fn &&fn) const {
        int cx = static_cast<int>(particles[i].position.x * invCellSize);
        int cy = static_cast<int>(particles[i].position.y * invCellSize);
        if (cx < 0) cx = 0;
        else if (cx >= cellsX) cx = cellsX - 1;
        if (cy < 0) cy = 0;
        else if (cy >= cellsY) cy = cellsY - 1;

        for (int dy = -1; dy <= 1; ++dy) {
            const int y = cy + dy;
            if (y < 0 || y >= cellsY) continue;
            const int row = y * cellsX;

            for (int dx = -1; dx <= 1; ++dx) {
                const int x = cx + dx;
                if (x < 0 || x >= cellsX) continue;

                int32_t p = cellHead[row + x];
                while (p != -1) {
                    fn(static_cast<uint32_t>(p));
                    p = next[p];
                }
            }
        }
    }

    float cellSize;
    float invCellSize;

    int worldWidth;
    int worldHeight;
    int cellsX;
    int cellsY;

    // Uniform grid as cell -> particle linked lists.
    // cellHead[cell] is the first particle index in that cell, next[i] is the next particle index.
    std::vector<int32_t> cellHead;
    std::vector<int32_t> next;

    std::vector<Particle> &particles;
};
