cmake_minimum_required(VERSION 3.22)
project(game_of_life)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

find_library(GTest required)
include_directories(${GTEST_INCLUDE_DIRS} ${SFML_INCLUDE_DIR})

set(CONWAY_SOURCE MooreNeighbor.h ConwayCell.h ConwayGrid.cpp ConwayGrid.h MooreNeighbor.cpp GOLFile.h GOLFile.cpp ConwayDefs.h)

add_executable(gol_unit_tests GOLTests.cpp ${CONWAY_SOURCE})
target_link_libraries(gol_unit_tests -lgtest -lgtest_main pthread)