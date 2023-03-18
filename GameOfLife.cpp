#include "GameOfLife.h"

#include <cmath>
#include <iostream>

namespace gol {
///
GameOfLife::GameOfLife(std::string_view pattern, ConwayGrid grid)
        : m_window(pattern, GOL_WINDOW_SIZE), m_conwayGrid(std::move(grid)) {
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
void GameOfLife::restartClock() {
    m_elapsed = m_clock.restart();
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
    for (auto& cell : m_conwayCells) {
        m_window.draw(cell);
    }
    m_window.endDraw();
}

///
void GameOfLife::generateGrid() {
    sf::Vector2f centroid{0.5f * m_window.getWindowSize().x, 0.5f * m_window.getWindowSize().y};

    auto numCellsH = m_conwayGrid.getGridWidth();
    auto numCellsV = m_conwayGrid.getGridHeight();

    auto totalWidth = numCellsH * GOL_CELL_DIMENSION;
    auto totalHeight = numCellsV * GOL_CELL_DIMENSION;

    CellArray cells = m_conwayGrid.getPendingGrid();
    sf::Vector2f gridStart = {centroid.x - totalWidth / 2, centroid.y - totalHeight / 2};
    for (auto i = 0; i < numCellsH; ++i) {
        for (auto j = 0; j < numCellsV; ++j) {
            sf::Vector2f cellPosition = {
                    gridStart.x + i * GOL_CELL_DIMENSION, gridStart.y + j * GOL_CELL_DIMENSION};
            std::cout << "Creating cell at " << cellPosition.x << ", " << cellPosition.y
                      << std::endl;

            auto currentCell = cells[i * numCellsV + j];
            m_conwayCells.push_back(genLifeCell(currentCell, cellPosition, GOL_CELL_SIZE));
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

    std::cout << "Setting alive state " << std::boolalpha << currentCell.isAlive() << std::endl;
    cell.setFillColor(m_cellColors[currentCell.getPendingState()]);

    return cell;
}

///
void GameOfLife::updateGrid() {
    CellArray cells = m_conwayGrid.getPendingGrid();
    auto gridit = m_conwayCells.begin();
    for (auto& cell : cells) {
        auto pendingState = cell.getPendingState();
        gridit->setFillColor(m_cellColors[pendingState]);
        ++gridit;
    }
}

}  // namespace gol