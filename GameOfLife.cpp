/// \file GameOfLife.h
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
#include "GameOfLife.h"

#include "GOLConfig.h"

#include <cassert>
#include <cmath>
#include <iostream>

namespace gol {
///
GameOfLife::GameOfLife(
        std::string_view patternName,
        ConwayGrid grid,
        ScreenSize screenSize,
        float tileSize,
        GOLConfig golConfig)
    : m_window(patternName, {screenSize.first, screenSize.second})
    , m_conwayGrid(std::move(grid))
    , m_tileSize(tileSize)
    , m_golConfig(std::move(golConfig)) {
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
    for (auto& cellRow : m_cellRectangles) {
        for (auto& cell : cellRow) {
            m_window.draw(cell);
        }
    }
    m_window.endDraw();
}

///
void GameOfLife::generateGrid() {
    auto gridHeight = m_conwayGrid.getGridHeight();
    auto gridWidth = m_conwayGrid.getGridWidth();

    auto totalWidth = gridWidth * m_tileSize;
    auto totalHeight = gridHeight * m_tileSize;

    sf::Vector2f centroid{0.5f * m_window.getWindowSize().x, 0.5f * m_window.getWindowSize().y};
    sf::Vector2f gridStart = {centroid.x - totalWidth / 2, centroid.y - totalHeight / 2};

    CellArray cellArray = m_conwayGrid.getPendingGrid();
    sf::Vector2f cellSize{m_tileSize - 1.0f, m_tileSize - 1.0f};
    auto row = 0;
    for (const auto& cellRow : cellArray) {
        auto col = 0;
        RectangleShapeRow shapeRow{};
        for (const auto& cell : cellRow) {
            sf::Vector2f cellPosition = {
                    gridStart.x + col * m_tileSize, gridStart.y + row * m_tileSize};
            shapeRow.push_back(genLifeCell(cell, cellPosition, cellSize));
            ++col;
        }
        m_cellRectangles.push_back(shapeRow);
        ++row;
    }
}

/// \note PRIVATE
sf::RectangleShape GameOfLife::genLifeCell(
        const ConwayCell& currentCell,
        const sf::Vector2f& cellPosition,
        const sf::Vector2f& cellSize) {
    sf::RectangleShape cell(cellSize);
    cell.setPosition(cellPosition);
    cell.setFillColor(sf::Color(m_golConfig.getCellColor(currentCell.getPendingState())));

    return cell;
}

///
void GameOfLife::updateGrid() {
    CellArray cells = m_conwayGrid.getPendingGrid();
    auto row = 0;
    for (auto& cellRow : m_cellRectangles) {
        auto col = 0;
        for (auto& cell : cellRow) {
            auto currentCell = cells[row][col];
            auto pendingState = currentCell.getPendingState();
            cell.setFillColor(sf::Color(m_golConfig.getCellColor(pendingState)));
            ++col;
        }
        ++row;
    }
}

}  // namespace gol