/// \file main.cpp
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

    PatternArray patternArray = patternFile.getPatternArray();
    std::clog << "Constructing Conway grid of " << patternArray.size() << " rows, "
              << patternArray[0].length() << " cols" << std::endl;

    ConwayGrid conwayGrid(patternArray);
    GameOfLife game(patternName, conwayGrid);

    auto counter = 0ul;
    while (!game.getWindow()->isDone()) {
        // Game loop.
        game.handleInput();
        game.update();
        game.render();
        auto elapsed = game.getElapsed();
        if (elapsed.asSeconds() >= 1.0) {
            std::cout << ++counter << std::endl;
            game.restartClock();
        }
    }
}