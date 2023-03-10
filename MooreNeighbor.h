/// \file MooreNeighbor.h
#pragma once
#include <vector>

namespace gol {
using NeighborArray = std::vector<std::pair<int, int>>;

NeighborArray mooreNeighborhood(int x, int y, int gridWidth, int gridHeight, bool wrapped = false) {
    NeighborArray neighbors;
    for (auto i = -1; i <= 1; i++) {
        for (auto j = -1; j <= 1; j++) {
            auto xCoord = x + i;
            auto yCoord = y + j;
            if (wrapped && (xCoord < 0 || xCoord == gridWidth || yCoord < 0 || yCoord == gridHeight)
                && (i != 0 || j != 0)) {
                neighbors.emplace_back(std::make_pair(xCoord, yCoord));
            } else if (
                    xCoord >= 0 && xCoord < gridWidth && yCoord >= 0 && yCoord < gridHeight
                    && !(i == 0 && j == 0)) {
                neighbors.emplace_back(std::make_pair(xCoord, yCoord));
            }
        }
    }
    return neighbors;
}
}  // namespace gol