/// \file ConwayGrid.cpp
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
#include "ConwayGrid.h"
#include "GOLFile.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

namespace gol {

///
ConwayGrid::ConwayGrid(int width, int height, bool wrapped)
    : m_width(width)
    , m_height(height)
    , m_wrapped(wrapped) {
    for (auto row = 0; row < m_width; ++row) {
        CellRow cellRow;
        for (auto col = 0; col < m_height; ++col) {
            cellRow.emplace_back(row, col, m_width, m_height, m_wrapped);
        }
        m_pending.push_back(cellRow);
    }

    m_snapshot.reserve(m_width * m_height);
}

///
ConwayGrid::ConwayGrid(PatternArray patternArray, ScreenSize padding, bool wrapped)
    : m_patternArray(std::move(patternArray))
    , m_width(patternArray[0].length())
    , m_height(patternArray.size())
    , m_padding(std::move(padding))
    , m_wrapped(wrapped) {

    fitGridToWindow();

    auto liveCount = 0;
    auto row = 0;
    for (const auto& patternRow : m_patternArray) {
        auto col = 0;
        CellRow cellRow;
        for (const auto& patternCol : patternRow) {
            bool isAlive = patternCol == PTEXT_LIVE;
            if (isAlive) {
                ++liveCount;
            }

            cellRow.emplace_back(col, row, m_width, m_height, isAlive, m_wrapped);
            ++col;
        }
        m_pending.push_back(cellRow);
        ++row;
    }

    std::clog << "Constructed a " << m_width << " by " << m_height << " grid, " << liveCount
              << " live cells, " << m_pending.size() << " cells overall" << std::endl;

    m_snapshot.reserve(m_width * m_height);
}

///
CellArray ConwayGrid::compute() {
    copyPendingToSnapshot();
    for (auto& cellRow : m_pending) {
        for (auto& cell : cellRow) {
            cell.isAlive(cell.computeNextState(m_snapshot));
        }
    }

    return m_pending;
}

///
void ConwayGrid::dumpPendingGrid() const {
    for (const auto& cellRow : m_pending) {
        for (const auto& cell : cellRow) {
            std::clog << (cell.isAlive() ? PTEXT_LIVE : PTEXT_DEAD);
        }
        std::clog << std::endl;
    }
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

///
CellArray ConwayGrid::getSnapshotGrid() const {
    return m_snapshot;
}

/// \note PRIVATE
void ConwayGrid::copyPendingToSnapshot() {
    m_snapshot.clear();
    std::copy(m_pending.begin(), m_pending.end(), std::back_inserter(m_snapshot));
    assert(m_pending == m_snapshot);
}

/// \note PRIVATE
void ConwayGrid::fitGridToWindow() {
    auto padWidth = static_cast<int>(m_padding.first - m_width);

    if (padWidth > 0) {
        auto padLeft = static_cast<int>(std::ceil(padWidth / 2));
        auto padRight = padWidth - padLeft;
        for (auto& row : m_patternArray) {
            row.insert(0, padLeft, PTEXT_DEAD);
            row.append(padRight, PTEXT_DEAD);
        }

        std::clog << "Padded pattern with " << padLeft << " left, " << padRight << " right"
                  << std::endl;

        m_width += padWidth;
    }

    auto padHeight = static_cast<int>(m_padding.second - m_height);
    if (padHeight > 0) {
        auto padTop = static_cast<int>(std::ceil(padHeight / 2));
        std::string padRow(m_width, PTEXT_DEAD);
        m_patternArray.insert(m_patternArray.begin(), padTop, padRow);

        auto padBottom = padHeight - padTop;
        for (auto i = 0; i < padBottom; ++i) {
            m_patternArray.push_back(padRow);
        }
        std::clog << "Padded pattern with " << padTop << " top, " << padBottom << " bottom"
                  << std::endl;

        m_height += padHeight;
    }
}

}  // namespace gol