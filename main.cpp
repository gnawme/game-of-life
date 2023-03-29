/// \file main.cpp
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
#include "GameOfLife.h"

#include <cxxopts.hpp>

#include <iostream>

using namespace gol;
///
int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "usage: game_of_life <pattern-file-name>" << std::endl;
        EXIT_FAILURE;
    }

    cxxopts::Options options("game_of_life", "Conway's Game of Life in C++ and SFML");
    options.add_options()("wrapped", "Set grid to wrap at its edges");
    auto result = options.parse(argc, argv);
    bool wrappedGrid = result["wrapped"].as<bool>();
    std::clog << "Setting " << (wrappedGrid ? "infinite" : "bounded") << " grid" << std::endl;

    std::string patternName(*++argv);
    GOLFile patternFile(patternName);
    std::clog << "Opened pattern file " << patternFile.getFilename() << std::endl;

    auto lastSlash = patternName.find_last_of('/');
    if (lastSlash != std::string::npos) {
        patternName = patternName.substr(lastSlash + 1);
    }

    PatternArray patternArray = patternFile.getPatternArray();
    if (patternArray.empty()) {
        std::cerr << "Couldn't read pattern file " << patternFile.getFilename() << std::endl;
        EXIT_FAILURE;
    }

    std::clog << "Read Conway grid of " << patternArray.size() << " rows, "
              << patternArray[0].length() << " cols" << std::endl;

    ConwayGrid conwayGrid(patternArray, GOL_TILING_720P, wrappedGrid);
    GameOfLife game(patternName, conwayGrid, GOL_SCREEN_720P, GOL_TILE_SIZE);

    while (!game.getWindow()->isDone()) {
        game.handleInput();

        constexpr sf::Time LIFE_TICK{sf::seconds(1.0)};
        if (game.getElapsed() >= LIFE_TICK) {
            game.update();
            game.render();
            game.restartClock();
        }
    }
}