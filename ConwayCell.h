/// \file ConwayCell.h
#pragma once
#include "ConwayDefs.h"
#include "MooreNeighbor.h"

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace gol {

enum CellPending { CELL_ASLEEP, CELL_LONELY, CELL_CHOKED, CELL_LIVING, CELL_REBORN };
static std::unordered_map<CellPending, std::string> PENDING_STATE{
        {CELL_ASLEEP, "CELL_ASLEEP"},
        {CELL_LONELY, "CELL_LONELY"},
        {CELL_CHOKED, "CELL_CHOKED"},
        {CELL_LIVING, "CELL_LIVING"},
        {CELL_REBORN, "CELL_REBORN"}};

class ConwayCell {
public:
    ConwayCell() = delete;

    ///
    ConwayCell(int cellX, int cellY, int gridW, int gridH, bool alive = false, bool wrapped = false)
        : m_cellX(cellX)
        , m_cellY(cellY)
        , m_gridW(gridW)
        , m_gridH(gridH)
        , m_isAlive(alive) {
        m_cellPending = m_isAlive ? CELL_LIVING : CELL_ASLEEP;
        m_neighbors = mooreNeighborhood(cellX, cellY, gridW, gridH, wrapped);
    }

    ///
    bool computeNextState(const CellArray& snapshot) {
        auto numLivingNeighbors = 0;
        for (const auto& [i, j] : m_neighbors) {
            auto index = i * m_gridW + j;
            if (snapshot[index].m_isAlive) {
                ++numLivingNeighbors;
            }
        }

        m_cellPending = setPendingState(numLivingNeighbors);
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
        case 4:
        case 5:
        case 6:
        case 7:
            pendingState = CELL_CHOKED;
            m_isAlive = false;
            break;
        default:
            std::cerr << "Shouldn't be here: alive: " << std::boolalpha << m_isAlive
                      << ", living neighbors: " << numLivingNeighbors << std::endl;
        }

        return pendingState;
    }

    int m_cellX{0};
    int m_cellY{0};
    int m_gridW{0};
    int m_gridH{0};
    bool m_isAlive{false};
    CellPending m_cellPending{CELL_ASLEEP};

    NeighborArray m_neighbors;
};
}  // namespace gol