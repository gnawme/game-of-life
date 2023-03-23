/// \file ConwayGrid.cpp
#include "ConwayGrid.h"
#include "GOLFile.h"

#include <algorithm>
#include <iostream>

namespace gol {

///
ConwayGrid::ConwayGrid(int width, int height, bool wrapped)
    : m_width(width)
    , m_height(height)
    , m_wrapped(wrapped) {
    for (auto row = 0; row < m_width; ++row) {
        for (auto col = 0; col < m_height; ++col) {
            m_pending.emplace_back(row, col, m_width, m_height, m_wrapped);
        }
        std::cout << std::endl;
    }

    m_snapshot.reserve(m_width * m_height);
}

///
ConwayGrid::ConwayGrid(PatternArray patternArray, int padding, bool wrapped)
    : m_patternArray(std::move(patternArray))
    , m_width(patternArray[0].length())
    , m_height(patternArray.size())
    , m_padding(padding)
    , m_wrapped(wrapped) {

    if (m_width != m_height) {
        squareConwayGrid();
    }

    if (padding != 0) {
        padConwayGrid();
    }

    auto liveCount = 0;
    auto row = 0;
    for (const auto& patternRow : m_patternArray) {
        auto col = 0;
        for (const auto& patternCol : patternRow) {
            bool isAlive = patternCol == PTEXT_LIVE;
            if (isAlive) {
                ++liveCount;
            }

            m_pending.emplace_back(row, col, m_width, m_height, isAlive, m_wrapped);
            ++col;
        }
        ++row;
    }

    std::clog << "Constructed a " << m_width << " by " << m_height << " grid, " << liveCount
              << " live cells, " << m_pending.size() << " cells overall" << std::endl;

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
void ConwayGrid::padConwayGrid() {
    auto totalPad = m_padding - m_width;

    if (totalPad > 0) {
        auto padLeft = totalPad / 2;
        auto padRight = totalPad - padLeft;
        for (auto& row : m_patternArray) {
            row.insert(0, padLeft, PTEXT_DEAD);
            row.append(padRight, PTEXT_DEAD);
        }

        m_width += totalPad;

        totalPad = m_padding - m_height;
        auto padTop = totalPad / 2;
        std::string padRow(m_width, PTEXT_DEAD);
        m_patternArray.insert(m_patternArray.begin(), padTop, padRow);

        auto padBottom = totalPad - padTop;
        for (auto i = 0; i < padBottom; ++i) {
            m_patternArray.push_back(padRow);
        }

        m_height += totalPad;
    }
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