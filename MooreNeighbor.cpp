/// \file MooreNeighbor.cpp
/// \author Norm Evangelista
/// \copyright (c) 2023 Norm Evangelista
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

///
NeighborArray mooreNeighborhood(int x, int y, int gridWidth, int gridHeight, bool wrapped) {
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
                    && (i != 0 || j != 0)) {
                neighbors.emplace_back(std::make_pair(xCoord, yCoord));
            }
        }
    }
    return neighbors;
}

}  // namespace gol
