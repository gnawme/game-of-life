/// \file ConwayGrid.cpp
#include "ConwayGrid.h"
#include "GOLFile.h"

namespace gol {

///
ConwayGrid::ConwayGrid(int width, int height, bool wrapped)
        : m_width(width), m_height(height), m_wrapped(wrapped) {
    for (auto row = 0; row < m_width; ++row) {
        for (auto col = 0; col < m_height; ++col) {
            m_pending.push_back({row, col, m_width, m_height, m_wrapped});
        }
    }

    m_snapshot.reserve(m_width * m_height);
    copyPendingToSnapshot();
}

///
ConwayGrid::ConwayGrid(const PatternArray& patternArray, bool wrapped)
        : m_width(patternArray[0].size()), m_height(patternArray.size()), m_wrapped(wrapped) {

    auto cellX = 0;
    for (const auto& patternRow : patternArray) {
        auto cellY = 0;
        for (const auto& patternCol : patternRow) {
            bool isAlive = patternCol == PTEXT_LIVE;
            m_pending.push_back({cellX, cellY, m_width, m_height, isAlive, m_wrapped});
            ++cellY;
        }
        ++cellX;
    }

    m_snapshot.reserve(m_width * m_height);
    copyPendingToSnapshot();
}

///
CellArray ConwayGrid::compute() {
    auto pending = m_pending.begin();
    for (auto& cell : m_snapshot) {
        auto cellIsAlive = cell.computeNextState(m_snapshot);
        pending->isAlive(cellIsAlive);
        ++pending;
    }

    copyPendingToSnapshot();

    return m_pending;
}

///
CellArray ConwayGrid::getPendingGrid() const {
    return m_pending;
}

/// \note PRIVATE
void ConwayGrid::copyPendingToSnapshot() {
    m_snapshot = m_pending;
}
}  // namespace gol