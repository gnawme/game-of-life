#pragma once
#include "Window.h"

class GameOfLife {
public:
    GameOfLife();
    ~GameOfLife();

    void handleInput();
    void update();
    void render();

    Window* getWindow();

    sf::Time getElapsed();
    void restartClock();

private:
    void MoveMushroom();

    Window m_window{};
    sf::Clock m_clock{};
    sf::Time m_elapsed{};

    sf::Texture m_mushroomTexture{};
    sf::Sprite m_mushroom{};
    sf::Vector2i m_increment{};
};