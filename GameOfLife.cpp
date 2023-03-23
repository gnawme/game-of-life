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
    cell.setOrigin({0.5f * cellSize.x, 0.5f * cellSize.y});
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