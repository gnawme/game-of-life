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
constexpr sf::Color COLOR_LIVING{0XFF6361FF};
constexpr sf::Color COLOR_REBORN{0XFFA600FF};

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