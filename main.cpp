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

#include <iostream>

using namespace gol;
///
int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "usage: game_of_life <pattern-file-name>" << std::endl;
    }

    std::string patternName(*++argv);
    GOLFile patternFile(patternName);
    std::clog << "Read pattern file " << patternFile.getFilename() << std::endl;

    auto lastSlash = patternName.find_last_of('/');
    if (lastSlash != std::string::npos) {
        patternName = patternName.substr(lastSlash + 1);
    }

    PatternArray patternArray = patternFile.getPatternArray();
    std::clog << "Read Conway grid of " << patternArray.size() << " rows, "
              << patternArray[0].length() << " cols" << std::endl;

    constexpr int STANDARD_GRID = 36;
    ConwayGrid conwayGrid(patternArray, STANDARD_GRID);
    GameOfLife game(patternName, conwayGrid);

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