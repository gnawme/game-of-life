/// \file MooreNeighbor.h
#pragma once
#include <vector>

namespace gol {
using NeighborArray = std::vector<std::pair<int, int>>;

NeighborArray mooreNeighborhood(int x, int y, int gridWidth, int gridHeight, bool wrapped = false);
}  // namespace gol