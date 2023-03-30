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
#include <optional>

using namespace gol;

namespace {
///
std::optional<float> fitPatternToScreenSize(const PatternArray& patternArray) {
    auto tileSize = GOL_TILE_SIZE;
    auto maxCols = patternArray[0].length() * tileSize;
    auto maxRows = patternArray.size() * tileSize;

    while (maxCols > GOL_SCREEN_720P.first || maxRows > GOL_SCREEN_720P.second) {
        tileSize /= 2;
        if (tileSize == 1) {
            return std::nullopt;
        }

        maxCols = patternArray[0].length() * tileSize;
        maxRows = patternArray.size() * tileSize;
    }

    return tileSize;
}
}  // namespace

///
int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "usage: game_of_life <pattern-file-name>" << std::endl;
        EXIT_FAILURE;
    }

    cxxopts::Options options("game_of_life", "Conway's Game of Life in C++ and SFML");
    options.add_options()(
            "wrapped",
            "Set grid to wrap at its edges",
            cxxopts::value<bool>()->default_value("false"));
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
    auto tileSize = fitPatternToScreenSize(patternArray);
    if (!tileSize.has_value()) {
        std::cerr << patternFile.getFilename() << " is larger than currently supported"
                  << std::endl;
        EXIT_FAILURE;
    }

    ScreenSize tiling = {
            static_cast<unsigned int>(GOL_SCREEN_720P.first / tileSize.value()),
            static_cast<unsigned int>(GOL_SCREEN_720P.second / tileSize.value())};

    ConwayGrid conwayGrid(patternArray, tiling, wrappedGrid);
    GameOfLife game(patternName, conwayGrid, GOL_SCREEN_720P, tileSize.value());

    auto* gameWindow = game.getWindow();
    while (!gameWindow->isDone()) {
        game.handleInput();

        constexpr sf::Time LIFE_TICK{sf::seconds(0.5)};
        if (game.getElapsed() >= LIFE_TICK) {
            game.update();
            game.render();
            game.restartClock();
        }
    }
}