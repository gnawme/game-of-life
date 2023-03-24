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
#pragma once
#include "ConwayGrid.h"
#include "Window.h"

#include <string>
#include <vector>

namespace gol {
using RectangleShapeVec = std::vector<sf::RectangleShape>;

constexpr sf::Vector2u GOL_WINDOW_SIZE{800, 600};

constexpr float GOL_CELL_DIMENSION{16.0};
constexpr sf::Vector2f GOL_CELL_SIZE{GOL_CELL_DIMENSION - 1.0, GOL_CELL_DIMENSION - 1.0};

constexpr sf::Color COLOR_ASLEEP{0X003F5CFF};
constexpr sf::Color COLOR_LONELY{0X58508DFF};
constexpr sf::Color COLOR_CHOKED{0XBC5090FF};
constexpr sf::Color COLOR_LIVING{0XFFA600FF};
constexpr sf::Color COLOR_REBORN{0XFF6361FF};

constexpr float GAME_INTERVAL{1.0};

///
class GameOfLife {
public:
    GameOfLife(std::string_view patternName, ConwayGrid grid);
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

    sf::RectangleShape genLifeCell(
            const ConwayCell& currentCell,
            const sf::Vector2f& centroid,
            const sf::Vector2f& cellSize);

    void updateGrid();

    Window m_window{};
    sf::Clock m_clock{};
    sf::Time m_elapsed{};
    RectangleShapeVec m_cellRectangles;
    ConwayGrid m_conwayGrid;
    std::unordered_map<CellPending, sf::Color> m_cellColors{
            {CELL_ASLEEP, COLOR_ASLEEP},
            {CELL_LONELY, COLOR_LONELY},
            {CELL_CHOKED, COLOR_CHOKED},
            {CELL_LIVING, COLOR_LIVING},
            {CELL_REBORN, COLOR_REBORN}};
};
}  // namespace gol