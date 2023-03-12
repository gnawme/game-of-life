/// \file ConwayCell.h
#pragma once
#include "ConwayDefs.h"
#include "MooreNeighbor.h"

#include <algorithm>
#include <vector>

namespace gol {

enum CellPending { CELL_DORMANT, CELL_LONELY, CELL_LIVING, CELL_CHOKED, CELL_REBORN };

class ConwayCell {
public:
    ConwayCell() = delete;

    ///
    ConwayCell(int cellX, int cellY, int gridW, int gridH, bool alive = false, bool wrapped = false)
            : m_cellX(cellX), m_cellY(cellY), m_gridW(gridW), m_gridH(gridH), m_isAlive(alive) {
        m_neighbors = mooreNeighborhood(cellX, cellY, gridW, gridH, wrapped);
    }

    ///
    bool computeNextState(const CellArray& snapshot) {
        auto numAlive = 0;
        for (const auto& [i, j] : m_neighbors) {
            auto index = i * m_gridW + j;
            if (snapshot[index].m_isAlive) {
                ++numAlive;
            }
        }

        m_cellPending = setPendingState(numAlive);
        return m_isAlive;
    }

    ///
    CellPending getPendingState() const {
        return m_cellPending;
    }

    ///
    std::size_t getNumNeighbors() const {
        return m_neighbors.size();
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
    CellPending setPendingState(int numAlive) {
        CellPending pendingState = CELL_DORMANT;

        if (numAlive == 3) {
            pendingState = m_isAlive ? CELL_LIVING : CELL_REBORN;
        } else if (numAlive >= 4 && numAlive <= 8) {
            if (m_isAlive) {
                m_isAlive = false;
                pendingState = CELL_CHOKED;
            }
        } else if (numAlive == 2 && m_isAlive) {
            pendingState = CELL_LIVING;
        } else if (numAlive <= 1 && m_isAlive) {
            m_isAlive = false;
            pendingState = CELL_LONELY;
        }

        return pendingState;
    }

    int m_cellX{0};
    int m_cellY{0};
    int m_gridW{0};
    int m_gridH{0};
    bool m_isAlive{false};
    CellPending m_cellPending{CELL_DORMANT};

    NeighborArray m_neighbors;
};
}  // namespace gol