/// \file ConwayCell.h
/// \author Norm Evangelista
/// \copyright (c) 2023 Norm Evangelista
// Copyright 2023 gnawme (Norm Evangelista)
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
#pragma once
#include "ConwayDefs.h"
#include "MooreNeighbor.h"

#include <algorithm>
#include <iostream>
#include <vector>

namespace gol {

///
class ConwayCell {
public:
    ConwayCell() = delete;

    ///
    ConwayCell(int cellX, int cellY, int gridW, int gridH, bool alive = false, bool wrapped = false)
        : m_cellX(cellX)
        , m_cellY(cellY)
        , m_gridW(gridW)
        , m_gridH(gridH)
        , m_isAlive(alive)
        , m_wrapped(wrapped)
        , m_pendingState(m_isAlive ? CELL_LIVING : CELL_ASLEEP) {
        m_neighbors = mooreNeighborhood(cellX, cellY, gridW, gridH, wrapped);
    }

    ///
    bool operator==(const ConwayCell& rhs) const {
        return (m_cellX == rhs.m_cellX && m_cellY == rhs.m_cellY && m_isAlive == rhs.m_isAlive
                && m_pendingState == rhs.m_pendingState);
    }

    ///
    bool computeNextState(const CellArray& snapshot) {
        auto numLivingNeighbors = 0;
        for (const auto& [row, col] : m_neighbors) {
            auto cell = snapshot[col][row];
            if (cell.m_isAlive) {
                ++numLivingNeighbors;
            }
        }

        m_pendingState = setPendingState(numLivingNeighbors);
        return m_isAlive;
    }

    ///
    CellPending getPendingState() const {
        return m_pendingState;
    }

    ///
    std::size_t getNumNeighbors() const {
        return m_neighbors.size();
    }

    std::pair<int, int> getRowCol() const {
        return {m_cellX, m_cellY};
    }

    ///
    bool isAlive() const {
        return m_isAlive;
    }

    ///
    void isAlive(bool alive) {
        m_isAlive = alive;
    }

private:
    ///
    CellPending setPendingState(int numLivingNeighbors) {
        CellPending pendingState = CELL_ASLEEP;

        switch (numLivingNeighbors) {
        case 0:
        case 1:
            if (m_isAlive) {
                m_isAlive = false;
                pendingState = CELL_LONELY;
            }
            break;
        case 2:
            pendingState = m_isAlive ? CELL_LIVING : CELL_ASLEEP;
            break;
        case 3:
            if (m_isAlive) {
                pendingState = CELL_LIVING;
            } else {
                pendingState = CELL_REBORN;
                m_isAlive = true;
            }
            break;
        default:
            pendingState = CELL_CHOKED;
            m_isAlive = false;
            break;
        }

        return pendingState;
    }

    int m_cellX{0};
    int m_cellY{0};
    int m_gridW{0};
    int m_gridH{0};
    bool m_isAlive{false};
    bool m_wrapped{false};
    CellPending m_pendingState{CELL_ASLEEP};

    NeighborArray m_neighbors;
};
}  // namespace gol