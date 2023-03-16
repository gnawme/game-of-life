#include "GameOfLife.h"

namespace gol {
///
GameOfLife::GameOfLife(std::string_view pattern, ConwayGrid grid) : m_window(pattern, GOL_WINDOW_SIZE), m_conwayGrid(std::move(grid)) {
    restartClock();

    m_conwayCells.push_back(genLifeCell(m_window.getWindowSize(), GOL_CELL_SIZE));

}

///
GameOfLife::~GameOfLife() = default;

///
sf::Time GameOfLife::getElapsed() {
    return m_elapsed;
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

/// \note PRIVATE
sf::RectangleShape GameOfLife::genLifeCell(const sf::Vector2u& windowSize, const sf::Vector2f& cellSize) {
    sf::RectangleShape cell(cellSize);
    cell.setOrigin({0.5f * cell.getSize().x, 0.5f * cell.getSize().y});
    cell.setFillColor(COLOR_DORMANT);

    return cell;
}

}  // namespace gol