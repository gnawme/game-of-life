#include "ConwayCell.h"
#include "ConwayGrid.h"
#include "GOLFile.h"

#include <gtest/gtest.h>

using namespace gol;

struct GOLTests : public ::testing::Test {
    void SetUp() override {
        myPatternArray = myGOLFile.readPatternFile("../block.rle");
    }

    GOLFile myGOLFile;
    PatternArray myPatternArray;

    static constexpr int CELL_X{1};
    static constexpr int CELL_Y{1};
    static constexpr int W3x3{3};
    static constexpr int H3x3{3};
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
    constexpr int W2x2{2};
    constexpr int H2x2{2};

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
    constexpr int W2x2{2};
    constexpr int H2x2{2};

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
TEST_F(GOLTests, ReadingPlaintextBlockShouldReturn2x2) {
    EXPECT_EQ(myPatternArray.size(), 2);
    EXPECT_EQ(myPatternArray[0].size(), 2);
}

///
TEST_F(GOLTests, ConstructingGridFromPatternShouldWork) {
    auto blockGrid = ConwayGrid(myPatternArray);

    auto pendingGrid = blockGrid.getPendingGrid();
    EXPECT_TRUE(!pendingGrid.empty());

    auto newGrid = blockGrid.compute();
    EXPECT_TRUE(!newGrid.empty());
    auto allAlive = true;
    for (const auto& cell : newGrid) {
        allAlive = allAlive && cell.isAlive();
    }
    EXPECT_TRUE(allAlive);
}