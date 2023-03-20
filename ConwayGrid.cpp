/// \file ConwayGrid.cpp
#include "ConwayGrid.h"
#include "GOLFile.h"

#include <algorithm>
#include <iostream>

namespace gol {

///
ConwayGrid::ConwayGrid(int width, int height, bool wrapped)
        : m_width(width), m_height(height), m_wrapped(wrapped) {
    for (auto row = 0; row < m_width; ++row) {
        for (auto col = 0; col < m_height; ++col) {
            m_pending.emplace_back(row, col, m_width, m_height, m_wrapped);
        }
    }

    m_snapshot.reserve(m_width * m_height);
}

///
ConwayGrid::ConwayGrid(PatternArray patternArray, bool wrapped)
        : m_patternArray(std::move(patternArray))
        , m_width(patternArray[0].length())
        , m_height(patternArray.size())
        , m_wrapped(wrapped) {

    if (m_width != m_height) {
        squareConwayGrid();
    }

    auto liveCount = 0;
    auto cellX = 0;
    for (const auto& patternRow : m_patternArray) {
        auto cellY = 0;
        for (const auto& patternCol : patternRow) {
            bool isAlive = patternCol == PTEXT_LIVE;
            if (isAlive) {
                ++liveCount;
            }
            m_pending.emplace_back(cellX, cellY, m_width, m_height, isAlive, m_wrapped);
            ++cellY;
        }
        ++cellX;
    }

    std::clog << "Constructed a " << m_width << " by " << m_height << " grid, " << liveCount
              << " live cells" << std::endl;

    m_snapshot.reserve(m_width * m_height);
}

///
CellArray ConwayGrid::compute() {
    copyPendingToSnapshot();
    for (auto& cell : m_pending) {
        auto cellIsAlive = cell.computeNextState(m_snapshot);
        cell.isAlive(cellIsAlive);
    }

    return m_pending;
}

///
int ConwayGrid::getGridHeight() const {
    return m_height;
}

///
int ConwayGrid::getGridWidth() const {
    return m_width;
}

///
CellArray ConwayGrid::getPendingGrid() const {
    return m_pending;
}

/// \note PRIVATE
void ConwayGrid::copyPendingToSnapshot() {
    m_snapshot.clear();
    std::copy(m_pending.begin(), m_pending.end(), std::back_inserter(m_snapshot));
}

/// \note PRIVATE
void ConwayGrid::squareConwayGrid() {
    auto diff = m_width - m_height;

    if (diff > 0) {
        std::string deadRow(m_patternArray[0].length(), PTEXT_DEAD);
        for (auto i = 1; i < diff + 1; ++i) {
            if (i % 2 == 0) {
                m_patternArray.push_back(deadRow);
            } else {
                m_patternArray.insert(m_patternArray.begin(), deadRow);
            }
        }
        m_height = m_width;
    } else {
        for (auto& padme : m_patternArray) {
            auto wdiff = -diff;
            for (auto i = 1; i < wdiff + 1; ++i) {
                if (i % 2 == 0) {
                    padme.push_back(PTEXT_DEAD);
                } else {
                    padme.insert(0, 1, PTEXT_DEAD);
                }
            }
        }
        m_width = m_height;
    }
}
}  // namespace gol