cmake_minimum_required(VERSION 3.15...3.28)
project(game-of-life VERSION 1.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Conan integration
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})


# Include FetchContent module
include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

# Fetch SFML 3.0 (latest master)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(SFML)

# Fetch effolkronium random
FetchContent_Declare(
    effolkronium_random
    GIT_REPOSITORY https://github.com/ilqvya/random.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(effolkronium_random)

# Fetch cxxopts
FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG        v3.2.1
)
FetchContent_MakeAvailable(cxxopts)


# Find Conan-provided packages
find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)

# Conway source files
set(CONWAY_SOURCE
    MooreNeighbor.h
    ConwayCell.h
    ConwayGrid.cpp
    ConwayGrid.h
    MooreNeighbor.cpp
    GOLFile.h
    GOLFile.cpp
    ConwayDefs.h
)

# Game source files
set(GAME_SOURCE
    GameOfLife.cpp
    GameOfLife.h
    Window.cpp
    Window.h
    main.cpp
    GOLConfig.cpp
    GOLConfig.h
)

# Set the path to test data (patterns directory)
set(TEST_DATA_DIR "${CMAKE_SOURCE_DIR}/patterns")

# Generate TestConfig.h from template
configure_file(
    ${CMAKE_SOURCE_DIR}/tests/TestConfig.h.in
    ${CMAKE_BINARY_DIR}/generated/TestConfig.h
    @ONLY
)

# Unit tests executable
add_executable(gol_unit_tests GOLTests.cpp ${CONWAY_SOURCE})
target_link_libraries(gol_unit_tests
    PRIVATE
    GTest::gtest
    GTest::gtest_main
    nlohmann_json::nlohmann_json
)
target_include_directories(gol_unit_tests
    PRIVATE
    ${CMAKE_BINARY_DIR}/generated  # For test_config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${effolkronium_random_SOURCE_DIR}/include
)

# Main game executable
add_executable(game_of_life ${CONWAY_SOURCE} ${GAME_SOURCE})
target_link_libraries(game_of_life
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    nlohmann_json::nlohmann_json
    cxxopts::cxxopts
)
target_include_directories(game_of_life
    PRIVATE
    ${effolkronium_random_SOURCE_DIR}/include
)