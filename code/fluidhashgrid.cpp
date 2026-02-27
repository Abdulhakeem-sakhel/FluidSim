#include "fluidhashgrid.h"

#include <algorithm>
#include <cstdint>
#include <limits>

FluidHashGrid::FluidHashGrid(float cellSize, std::vector<Particle> &particles)
    : cellSize(cellSize),
      invCellSize(cellSize > 0.0f ? (1.0f / cellSize) : 0.0f),
      worldWidth(0),
      worldHeight(0),
      cellsX(0),
      cellsY(0),
      particles(particles) {
}

void FluidHashGrid::setWorldSize(int width, int height) {
    width = std::max(width, 1);
    height = std::max(height, 1);

    // Compute uniform grid resolution.
    // +1 so particles exactly on the boundary still map to a valid cell.
    const int newCellsX = std::max(1, static_cast<int>(width * invCellSize) + 1);
    const int newCellsY = std::max(1, static_cast<int>(height * invCellSize) + 1);

    if (width == worldWidth && height == worldHeight && newCellsX == cellsX && newCellsY == cellsY) {
        return;
    }

    worldWidth = width;
    worldHeight = height;
    cellsX = newCellsX;
    cellsY = newCellsY;

    cellHead.assign(static_cast<size_t>(cellsX * cellsY), -1);
}

void FluidHashGrid::clearGrid() {
    // Dense grid for a fixed-size domain: clearing by fill is fast.
    std::fill(cellHead.begin(), cellHead.end(), -1);
}

void FluidHashGrid::mapParticleToCell() {
    if (cellsX <= 0 || cellsY <= 0) return;

    if (next.size() != particles.size()) {
        next.assign(particles.size(), -1);
    }

    const int maxX = cellsX - 1;
    const int maxY = cellsY - 1;

    for (uint32_t i = 0; i < static_cast<uint32_t>(particles.size()); ++i) {
        const Vector2 p = particles[i].position;

        int x = static_cast<int>(p.x * invCellSize);
        int y = static_cast<int>(p.y * invCellSize);

        // Clamp to grid.
        if (x < 0) x = 0;
        else if (x > maxX) x = maxX;
        if (y < 0) y = 0;
        else if (y > maxY) y = maxY;

        const int cell = x + y * cellsX;
        next[i] = cellHead[cell];
        cellHead[cell] = static_cast<int32_t>(i);
    }
}

