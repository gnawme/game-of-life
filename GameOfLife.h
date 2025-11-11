/// \file GameOfLife.h
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
#pragma once
#include "ConwayGrid.h"
#include "GOLConfig.h"
#include "Window.h"

#include <string>
#include <vector>

namespace gol {
using RectangleShapeRow = std::vector<sf::RectangleShape>;
using RectangleShapeArray = std::vector<RectangleShapeRow>;

constexpr float GAME_INTERVAL{1.0};

///
class GameOfLife {
public:
    GameOfLife(
            std::string& patternName,
            ConwayGrid grid,
            ScreenSize screenSize,
            float tileSize,
            const GOLConfig& golConfig);
    ~GameOfLife();

    void handleInput();
    void update();
    void render();

    Window* getWindow();

    sf::Time getElapsed();
    void resetClock();
    void restartClock();

private:
    void generateGrid();

    void genCellColor(const ConwayCell& currentCell, sf::RectangleShape& cell);

    sf::RectangleShape genLifeCell(
            const ConwayCell& currentCell,
            const sf::Vector2f& cellPosition,
            const sf::Vector2f& cellSize);

    void updateGrid();

    Window m_window;
    ConwayGrid m_conwayGrid;
    float m_tileSize{16.0};
    GOLConfig m_golConfig;

    sf::Clock m_clock{};
    sf::Time m_elapsed;
    RectangleShapeArray m_cellRectangles;
    bool m_classicMode{false};
};
}  // namespace gol