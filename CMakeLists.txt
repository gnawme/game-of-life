cmake_minimum_required(VERSION 3.22)
project(game_of_life)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

find_library(GTest required)
include_directories(${GTEST_INCLUDE_DIRS} ${SFML_INCLUDE_DIR})

set(CONWAY_SOURCE MooreNeighbor.h ConwayCell.h ConwayGrid.cpp ConwayGrid.h MooreNeighbor.cpp GOLFile.h GOLFile.cpp ConwayDefs.h)
set(GAME_SOURCE GameOfLife.cpp GameOfLife.h Window.cpp Window.h)

add_executable(gol_unit_tests GOLTests.cpp ${CONWAY_SOURCE})
target_link_libraries(gol_unit_tests -lgtest -lgtest_main pthread)

add_executable(game_of_life ${CONWAY_SOURCE} ${GAME_SOURCE})
target_link_libraries(game_of_life sfml-system sfml-window sfml-graphics)
target_include_directories(game_of_life
        PUBLIC
        "/usr/local/include")

target_link_directories(game_of_life
        PUBLIC
        "/usr/local/lib")
