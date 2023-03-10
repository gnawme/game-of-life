cmake_minimum_required(VERSION 3.24)
project(game_of_life)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

find_library(GTest required)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(game_of_life GOLTests.cpp MooreNeighbor.h ConwayCell.h)
target_link_libraries(game_of_life -lgtest -lgtest_main pthread)