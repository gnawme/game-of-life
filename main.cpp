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
    std::clog << "Read Conway grid of " << patternArray.size() << " rows, "
              << patternArray[0].length() << " cols" << std::endl;

    ConwayGrid conwayGrid(patternArray);
    GameOfLife game(patternName, conwayGrid);

    constexpr sf::Time LIFE_TICK{sf::seconds(1.0)};

    while (!game.getWindow()->isDone()) {
        game.handleInput();

        if (game.getElapsed() >= LIFE_TICK) {
            game.update();
            game.render();
            game.restartClock();
        }
    }
}