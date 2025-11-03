/// \file main.cpp
/// \author Norm Evangelista
/// \copyright (c) 2025 Norm Evangelista
// Copyright 2025 gnawme (Norm Evangelista)
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
#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"

#include <cstdint>
#include <cstdlib>
#include <cxxopts.hpp>

#include <chrono>
#include <exception>
#include <filesystem>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

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
    namespace fs = std::filesystem;

    // Check if file exists before attempting to open
    if (!fs::exists(patternName)) {
        std::cerr << "Error: Pattern file does not exist: " << patternName << std::endl;
        return std::nullopt;
    }

    if (!fs::is_regular_file(patternName)) {
        std::cerr << "Error: Path is not a regular file: " << patternName << std::endl;
        return std::nullopt;
    }

    GOLFile patternFile(patternName);
    std::clog << "Opened pattern file " << patternFile.getFilename() << std::endl;

    // Extract just the filename for the window title
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

///
void printUsage() {
    std::cout << "\n";
    std::cout << "Conway's Game of Life - Modern C++ Implementation with SFML Graphics\n";
    std::cout << "====================================================================\n\n";
    std::cout << "USAGE:\n";
    std::cout << "  game_of_life <pattern-file> [OPTIONS]\n";
    std::cout << "  game_of_life --random [OPTIONS]\n\n";
    std::cout << "DESCRIPTION:\n";
    std::cout << "  Simulates Conway's Game of Life with colorized cell transitions.\n";
    std::cout << "  Supports plaintext (.cells) and RLE (.rle) pattern formats.\n\n";
    std::cout << "ARGUMENTS:\n";
    std::cout << "  <pattern-file>    Path to a pattern file (.cells or .rle format)\n";
    std::cout << "                    Example: ./patterns/glider.cells\n\n";
    std::cout << "OPTIONS:\n";
    std::cout << "  --random          Generate a random soup instead of loading a pattern\n";
    std::cout << "  --wrapped         Enable wrapped/toroidal grid (infinite plane)\n";
    std::cout << "                    Default: bounded grid with edges\n";
    std::cout << "  -h, --help        Display this help message and exit\n\n";
    std::cout << "INTERACTIVE CONTROLS:\n";
    std::cout << "  F5                Toggle fullscreen mode\n";
    std::cout << "  ESC               Exit the simulation\n\n";
    std::cout << "CONFIGURATION:\n";
    std::cout << "  Display parameters and cell colors can be customized via gol_config.json\n";
    std::cout << "  in the application directory.\n\n";
    std::cout << "EXAMPLES:\n";
    std::cout << "  # Run with a pattern file\n";
    std::cout << "  game_of_life patterns/glider.cells\n\n";
    std::cout << "  # Run with wrapped grid (infinite plane)\n";
    std::cout << "  game_of_life patterns/gosper_glider_gun.rle --wrapped\n\n";
    std::cout << "  # Generate a random soup\n";
    std::cout << "  game_of_life --random\n\n";
    std::cout << "  # Random soup with wrapped grid\n";
    std::cout << "  game_of_life --random --wrapped\n\n";
    std::cout << "PATTERN FORMATS:\n";
    std::cout << "  Plaintext (.cells): https://conwaylife.com/wiki/Plaintext\n";
    std::cout << "  RLE (.rle):         https://conwaylife.com/wiki/Run_Length_Encoded\n\n";
}
}  // namespace

///
int main(int argc, char** argv) {
    try {
        cxxopts::Options options("game_of_life", "Conway's Game of Life in C++ and SFML");

        options.add_options()("h,help", "Show help message")(
                "wrapped",
                "Set grid to wrap at its edges (infinite plane)",
                cxxopts::value<bool>()->default_value("false"))(
                "random", "Create a random soup", cxxopts::value<bool>()->default_value("false"))(
                "pattern", "Pattern file to load", cxxopts::value<std::string>())(
                "positional", "Positional arguments", cxxopts::value<std::vector<std::string>>());

        options.parse_positional({"pattern", "positional"});
        options.positional_help("<pattern-file>");

        auto result = options.parse(argc, argv);

        // Handle help request
        if (result.count("help") || argc == 1) {
            printUsage();
            return (argc == 1) ? EXIT_FAILURE : EXIT_SUCCESS;
        }

        bool wrappedGrid = result["wrapped"].as<bool>();
        std::clog << "Setting " << (wrappedGrid ? "infinite" : "bounded") << " grid" << std::endl;

        bool randomSoup = result["random"].as<bool>();
        std::string patternName;

        if (randomSoup) {
            std::clog << "Random soup requested" << std::endl;
            std::stringstream ss;
            ss << "soup_" << getTimestamp();
            patternName = ss.str();
        } else {
            // Check if pattern file was provided
            if (!result.count("pattern")) {
                std::cerr << "Error: No pattern file specified and --random not used\n"
                          << std::endl;
                printUsage();
                return EXIT_FAILURE;
            }

            patternName = result["pattern"].as<std::string>();
        }

        GOLConfig golConfig;
        auto gridTiling = !randomSoup
                ? generateGridFromPatternFile(patternName, golConfig, wrappedGrid)
                : generateRandomSoup(golConfig, wrappedGrid);

        if (!gridTiling.has_value()) {
            std::cerr << "Failed to initialize game grid" << std::endl;
            return EXIT_FAILURE;
        }

        auto& [conwayGrid, tileSize] = gridTiling.value();
        golConfig.setTileSize(tileSize);
        GameOfLife game(patternName, conwayGrid, golConfig.getScreenSize(), tileSize, golConfig);

        auto* gameWindow = game.getWindow();
        game.render();

        sf::Clock startupClock;
        float startupDelay = golConfig.getStartupDelay();
        while (startupClock.getElapsedTime().asSeconds() < startupDelay && !gameWindow->isDone()) {
            game.handleInput();
            game.render();

            constexpr std::int32_t SPINNER_MS{16};
            sf::sleep(sf::milliseconds(SPINNER_MS));
        }

        while (!gameWindow->isDone()) {
            game.handleInput();

            sf::Time lifeTick{sf::seconds(golConfig.getLifeTick())};
            if (game.getElapsed() >= lifeTick) {
                game.update();
                game.render();
                game.restartClock();
            }
        }

        return EXIT_SUCCESS;

    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing options: " << e.what() << "\n" << std::endl;
        printUsage();
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}