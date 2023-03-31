/// \file GOLTests.cpp
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
#include "ConwayCell.h"
#include "ConwayGrid.h"
#include "GOLFile.h"

#include <gtest/gtest.h>

using namespace gol;

struct GOLTests : public ::testing::Test {
    void SetUp() override {
        myPatternArray = myGOLFile.readPatternFile("../block.cells");
    }

    GOLFile myGOLFile;
    PatternArray myPatternArray;

    static constexpr int CELL_X{1};
    static constexpr int CELL_Y{1};
    static constexpr int W3x3{3};
    static constexpr int H3x3{3};
    static constexpr int W2x2{2};
    static constexpr int H2x2{2};
    static constexpr int W3x2{2};
    static constexpr int H3x2{3};
};

///
TEST_F(GOLTests, MooreNeighborhoodOf3x3ShouldBe8) {
    NeighborArray neighbors3x3 = mooreNeighborhood(CELL_X, CELL_Y, W3x3, H3x3);
    EXPECT_EQ(neighbors3x3.size(), 8);
}

///
TEST_F(GOLTests, NeighborhoodOfCenterCellIsNotEmpty) {
    ConwayCell myCCell(CELL_X, CELL_Y, W3x3, H3x3);
    EXPECT_EQ(myCCell.getNumNeighbors(), 8);
}

///
TEST_F(GOLTests, NeighborhoodOfCornerCellsShouldBe3) {
    ConwayCell ul(0, 0, W2x2, H2x2);
    EXPECT_EQ(ul.getNumNeighbors(), 3);
    ConwayCell ur(1, 0, W2x2, H2x2);
    EXPECT_EQ(ur.getNumNeighbors(), 3);
    ConwayCell lr(1, 1, W2x2, H2x2);
    EXPECT_EQ(lr.getNumNeighbors(), 3);
    ConwayCell ll(0, 1, W2x2, H2x2);
    EXPECT_EQ(ll.getNumNeighbors(), 3);
}

///
TEST_F(GOLTests, NeighborhoodOfWrappedCornerCellsShouldBe8) {
    ConwayCell ul(0, 0, W2x2, H2x2, false, true);
    EXPECT_EQ(ul.getNumNeighbors(), 8);
    ConwayCell ur(1, 0, W2x2, H2x2, false, true);
    EXPECT_EQ(ur.getNumNeighbors(), 8);
    ConwayCell lr(1, 1, W2x2, H2x2, false, true);
    EXPECT_EQ(lr.getNumNeighbors(), 8);
    ConwayCell ll(0, 1, W2x2, H2x2, false, true);
    EXPECT_EQ(ll.getNumNeighbors(), 8);
}

///
TEST_F(GOLTests, NeighborhoodOfEdgeCellsShouldBeComputedProperly) {
    ConwayCell tl(0, 0, W3x2, H3x2);
    EXPECT_EQ(tl.getNumNeighbors(), 3);
    ConwayCell tr(1, 0, W3x2, H3x2);
    EXPECT_EQ(tr.getNumNeighbors(), 3);

    ConwayCell ml(0, 1, W3x2, H3x2);
    EXPECT_EQ(ml.getNumNeighbors(), 5);
    ConwayCell mr(1, 1, W3x2, H3x2);
    EXPECT_EQ(mr.getNumNeighbors(), 5);
    ConwayCell ll(0, 2, W3x2, H3x2);
    EXPECT_EQ(ll.getNumNeighbors(), 3);
    ConwayCell lr(1, 2, W3x2, H3x2);
    EXPECT_EQ(lr.getNumNeighbors(), 3);
}

///
TEST_F(GOLTests, ReadingPlaintextBlockShouldReturn2x2) {
    EXPECT_EQ(myPatternArray.size(), 2);
    EXPECT_EQ(myPatternArray[0].size(), 2);
}

///
TEST_F(GOLTests, ConstructingGridFromPatternShouldWork) {
    auto blockGrid = ConwayGrid(myPatternArray, {2, 2});

    auto pendingGrid = blockGrid.getPendingGrid();
    EXPECT_TRUE(!pendingGrid.empty());

    auto newGrid = blockGrid.compute();
    EXPECT_TRUE(!newGrid.empty());
    auto allAlive = true;
    for (const auto& cellRow : newGrid) {
        for (const auto& cell : cellRow) {
            allAlive = allAlive && cell.isAlive();
        }
    }
    EXPECT_TRUE(allAlive);
}

///
TEST_F(GOLTests, ConstructingRandomSoupShouldWork) {
    auto soupGrid = ConwayGrid(GOL_TILING_720P);
    EXPECT_TRUE(soupGrid.getGridWidth() == GOL_TILING_720P.first);
    EXPECT_TRUE(soupGrid.getGridHeight() == GOL_TILING_720P.second);
}