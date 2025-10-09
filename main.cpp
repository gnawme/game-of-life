/// \file main.cpp
/// \author Norm Evangelista
/// \copyright (c) 2023 Norm Evangelista
// Copyright 2023 gnawme (Norm Evangelista)
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
#include "ConwayDefs.h"
#include "ConwayGrid.h"
#include "GOLConfig.h"
#include "GOLFile.h"
#include "GameOfLife.h"
#include "SFML/Config.hpp"
#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"

#include <cstdint>
#include <cstdlib>
#include <cxxopts.hpp>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <utility>

using namespace gol;

namespace {
///
std::optional<float>
fitPatternToScreenSize(const PatternArray& patternArray, GOLConfig& golConfig) {
    auto numCols = patternArray[0].length();
    auto numRows = patternArray.size();

    auto tileSize = golConfig.getTileSize();
    auto maxCols = numCols * tileSize;
    auto maxRows = patternArray.size() * tileSize;

    ScreenSize screenSize = golConfig.getScreenSize();

    while (maxCols > screenSize.first || maxRows > screenSize.second) {
        tileSize /= 2;
        if (tileSize == 1) {
            return std::nullopt;
        }

        maxCols = numCols * tileSize;
        maxRows = numRows * tileSize;
    }

    return tileSize;
}

///
std::optional<std::pair<ConwayGrid, float>>
generateGridFromPatternFile(std::string& patternName, GOLConfig& golConfig, bool wrappedGrid) {
    GOLFile patternFile(patternName);
    std::clog << "Opened pattern file " << patternFile.getFilename() << std::endl;

    namespace fs = std::filesystem;
    fs::path patternPath(patternName);
    patternName = patternPath.filename().string();

    PatternArray patternArray = patternFile.getPatternArray();
    if (patternArray.empty()) {
        std::cerr << "Couldn't read pattern file " << patternFile.getFilename() << std::endl;
        return std::nullopt;
    }

    std::clog << "Read Conway grid of " << patternArray.size() << " rows, "
              << patternArray[0].length() << " cols" << std::endl;
    auto tileSize = fitPatternToScreenSize(patternArray, golConfig);
    if (!tileSize.has_value()) {
        std::cerr << patternFile.getFilename() << " is larger than currently supported"
                  << std::endl;
        return std::nullopt;
    }

    ScreenSize screenSize = golConfig.getScreenSize();
    ScreenSize tiling = {
            static_cast<unsigned int>(screenSize.first / tileSize.value()),
            static_cast<unsigned int>(screenSize.second / tileSize.value())};

    return std::make_pair(ConwayGrid(patternArray, tiling, wrappedGrid), tileSize.value());
}

///
std::optional<std::pair<ConwayGrid, float>>
generateRandomSoup(GOLConfig& golConfig, bool wrappedGrid) {
    return std::make_pair(
            ConwayGrid(golConfig.getScreenTiling(), wrappedGrid), golConfig.getTileSize());
}

///
std::uint64_t getTimestamp() {
    return std::chrono::duration_cast<std::chrono::seconds>(
                   std::chrono::system_clock::now().time_since_epoch())
            .count();
}
}  // namespace

///
int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "usage: game_of_life <pattern-file-name> [--wrapped]" << std::endl;
        EXIT_FAILURE;
    }

    std::clog << "SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "."
              << SFML_VERSION_PATCH << std::endl;

    cxxopts::Options options("game_of_life", "Conway's Game of Life in C++ and SFML");
    options.add_options()(
            "wrapped",
            "Set grid to wrap at its edges",
            cxxopts::value<bool>()->default_value("false"))(
            "random", "Create a random soup", cxxopts::value<bool>()->default_value("false"));

    auto result = options.parse(argc, argv);

    std::string patternName(*++argv);

    bool wrappedGrid = result["wrapped"].as<bool>();
    std::clog << "Setting " << (wrappedGrid ? "infinite" : "bounded") << " grid" << std::endl;

    bool randomSoup = false;
    if (result.count("random") != 0) {
        std::clog << "Random soup requested" << std::endl;
        randomSoup = result["random"].as<bool>();
        std::stringstream ss;
        ss << "soup_" << getTimestamp();
        patternName = ss.str();
    }

    GOLConfig golConfig;
    auto gridTiling = !randomSoup ? generateGridFromPatternFile(patternName, golConfig, wrappedGrid)
                                  : generateRandomSoup(golConfig, wrappedGrid);

    if (!gridTiling.has_value()) {
        EXIT_FAILURE;
    }

    auto& [conwayGrid, tileSize] = gridTiling.value();
    golConfig.setTileSize(tileSize);
    GameOfLife game(patternName, conwayGrid, golConfig.getScreenSize(), tileSize, golConfig);

    auto* gameWindow = game.getWindow();
    game.render();
    sf::sleep(sf::seconds(1.0f));

    while (!gameWindow->isDone()) {
        game.handleInput();

        sf::Time lifeTick{sf::seconds(golConfig.getLifeTick())};
        if (game.getElapsed() >= lifeTick) {
            game.update();
            game.render();
            game.restartClock();
        }
    }
}