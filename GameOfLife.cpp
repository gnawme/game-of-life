/// \file GameOfLife.h
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
#include "GameOfLife.h"

#include <cassert>
#include <cmath>
#include <iostream>

namespace gol {
///
GameOfLife::GameOfLife(std::string_view patternName, ConwayGrid grid)
    : m_window(patternName, GOL_WINDOW_SIZE)
    , m_conwayGrid(std::move(grid)) {
    restartClock();
    generateGrid();
}

///
GameOfLife::~GameOfLife() = default;

///
sf::Time GameOfLife::getElapsed() {
    return m_clock.getElapsedTime();
}

///
void GameOfLife::resetClock() {
    m_elapsed -= sf::seconds(GAME_INTERVAL);
}

///
void GameOfLife::restartClock() {
    m_elapsed += m_clock.restart();
}

///
Window* GameOfLife::getWindow() {
    return &m_window;
}

///
void GameOfLife::handleInput() {
    // Input handling.
}

///
void GameOfLife::update() {
    m_conwayGrid.compute();
    updateGrid();
    m_window.update();
}

///
void GameOfLife::render() {
    m_window.beginDraw();
    for (auto& cell : m_cellRectangles) {
        m_window.draw(cell);
    }
    m_window.endDraw();
}

///
void GameOfLife::generateGrid() {
    sf::Vector2f centroid{0.5f * m_window.getWindowSize().x, 0.5f * m_window.getWindowSize().y};

    auto numRowCells = m_conwayGrid.getGridWidth();
    auto numColCells = m_conwayGrid.getGridHeight();

    auto totalWidth = numRowCells * GOL_CELL_DIMENSION;
    auto totalHeight = numColCells * GOL_CELL_DIMENSION;
    sf::Vector2f gridStart = {centroid.x - totalWidth / 2, centroid.y - totalHeight / 2};

    CellArray cells = m_conwayGrid.getPendingGrid();
    for (auto row = 0; row < numRowCells; ++row) {
        for (auto col = 0; col < numColCells; ++col) {
            sf::Vector2f cellPosition = {
                    gridStart.x + col * GOL_CELL_DIMENSION, gridStart.y + row * GOL_CELL_DIMENSION};

            auto currentCell = cells[row * numColCells + col];
            m_cellRectangles.push_back(genLifeCell(currentCell, cellPosition, GOL_CELL_SIZE));
        }
    }
}

/// \note PRIVATE
sf::RectangleShape GameOfLife::genLifeCell(
        const ConwayCell& currentCell,
        const sf::Vector2f& centroid,
        const sf::Vector2f& cellSize) {
    sf::RectangleShape cell(cellSize);
    cell.setPosition(centroid);
    cell.setFillColor(m_cellColors[currentCell.getPendingState()]);

    return cell;
}

///
void GameOfLife::updateGrid() {
    CellArray cells = m_conwayGrid.getPendingGrid();
    auto gridit = m_cellRectangles.begin();
    for (auto& cell : cells) {
        auto pendingState = cell.getPendingState();
        gridit->setFillColor(m_cellColors[pendingState]);
        ++gridit;
    }
}

}  // namespace gol