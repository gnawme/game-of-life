///
#pragma once

#include "MooreNeighbor.h"

#include <vector>

namespace gol {
enum CellLife { CELL_DORMANT, CELL_LONELY, CELL_LIVING, CELL_CHOKED, CELL_REBORN };

class ConwayCell {
public:
    ConwayCell() = delete;
    ConwayCell(int cellX, int cellY, int gridW, int gridH, bool wrapped = false)
            : m_cellX(cellX), m_cellY(cellY) {
        m_neighbors = mooreNeighborhood(cellX, cellY, gridW, gridH, wrapped);
    }

    std::size_t getNumNeighbors() const {
        return m_neighbors.size();
    }

    bool isAlive() const {
        return m_isAlive;
    }

private:
    int m_cellX{0};
    int m_cellY{0};
    bool m_isAlive{false};
    CellLife m_nextLife{CELL_DORMANT};

    NeighborArray m_neighbors;
};
}  // namespace gol