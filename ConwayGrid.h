/// \file ConwayGrid.h
#pragma once
#include "ConwayCell.h"
#include "ConwayDefs.h"

#include <vector>

namespace gol {

class ConwayGrid {
public:
    ConwayGrid(int width, int height, bool wrapped = false);
    ConwayGrid(const PatternArray& patternArray, bool wrapped = false);

    ConwayGrid() = delete;

    CellArray compute();

    int getGridHeight() const;
    int getGridWidth() const;
    CellArray getPendingGrid() const;

private:
    void copyPendingToSnapshot();

    int m_width{10};
    int m_height{10};
    bool m_wrapped{false};
    CellArray m_pending{};
    CellArray m_snapshot{};
};
}  // namespace gol