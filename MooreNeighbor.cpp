/// \file MooreNeighbor.cpp
/// \author Norm Evangelista
/// \copyright (c) 2025 Norm Evangelista
// Copyright 2025 gnawme (Norm Evangelista)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include "MooreNeighbor.h"

namespace gol {

/// \note Moore Neighborhood implementation
/// \see https://en.wikipedia.org/wiki/Moore_neighborhood
NeighborArray mooreNeighborhood(int col, int row, int gridWidth, int gridHeight, bool wrapped) {
    NeighborArray neighbors;
    neighbors.reserve(8);  // Preallocate for 8 neighbors

    if (wrapped) {
        // Wrapped grid: all 8 neighbors always valid
        for (auto dCol = -1; dCol <= 1; ++dCol) {
            for (auto dRow = -1; dRow <= 1; ++dRow) {
                if (dCol != 0 || dRow != 0) {
                    // Proper modulo for negative numbers
                    auto neighborCol = ((col + dCol) % gridWidth + gridWidth) % gridWidth;
                    auto neighborRow = ((row + dRow) % gridHeight + gridHeight) % gridHeight;
                    neighbors.emplace_back(neighborCol, neighborRow);
                }
            }
        }
    } else {
        // Bounded grid: check bounds
        for (auto dCol = -1; dCol <= 1; ++dCol) {
            for (auto dRow = -1; dRow <= 1; ++dRow) {
                if (dCol != 0 || dRow != 0) {
                    auto neighborCol = col + dCol;
                    auto neighborRow = row + dRow;
                    if (neighborCol >= 0 && neighborCol < gridWidth && neighborRow >= 0
                        && neighborRow < gridHeight) {
                        neighbors.emplace_back(neighborCol, neighborRow);
                    }
                }
            }
        }
    }

    return neighbors;
}
}  // namespace gol
