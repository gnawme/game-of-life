#pragma once
#include "ConwayGrid.h"
#include "Window.h"

#include <string>
#include <vector>

namespace gol {
using ConwayCellVector = std::vector<sf::RectangleShape>;

constexpr sf::Vector2u GOL_WINDOW_SIZE{800, 600};

constexpr float GOL_CELL_DIMENSION{16.0};
constexpr sf::Vector2f GOL_CELL_SIZE{GOL_CELL_DIMENSION - 1.0, GOL_CELL_DIMENSION - 1.0};

constexpr sf::Color COLOR_DORMANT{0x003f5cff};
constexpr sf::Color COLOR_LONELY{0x58508dff};
constexpr sf::Color COLOR_CHOKED{0xbc5090ff};
constexpr sf::Color COLOR_LIVING{0xff6361ff};
constexpr sf::Color COLOR_REBORN{0xffa600ff};

///
class GameOfLife {
public:
    GameOfLife(std::string_view pattern, ConwayGrid grid);
    ~GameOfLife();

    void handleInput();
    void update();
    void render();

    Window* getWindow();

    sf::Time getElapsed();
    void restartClock();

private:
    sf::RectangleShape genLifeCell(const sf::Vector2u& windowSize, const sf::Vector2f& cellSize);
    Window m_window{};
    sf::Clock m_clock{};
    sf::Time m_elapsed{};
    ConwayCellVector m_conwayCells;
    ConwayGrid m_conwayGrid;
};
}  // namespace gol