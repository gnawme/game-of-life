#include "GameOfLife.h"

///
GameOfLife::GameOfLife() : m_window("Chapter 2", {800, 600}) {
    restartClock();
    srand(time(nullptr));

    // Setting up class members.
    auto loaded = m_mushroomTexture.loadFromFile("Mushroom.png");
    if (loaded) {
        m_mushroom.setTexture(m_mushroomTexture);
        m_increment = {400, 400};
    }
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
    MoveMushroom();
}

///
void GameOfLife::MoveMushroom() {
    sf::Vector2u l_windSize = m_window.getWindowSize();
    sf::Vector2u l_textSize = m_mushroomTexture.getSize();

    if ((m_mushroom.getPosition().x > l_windSize.x - l_textSize.x && m_increment.x > 0)
        || (m_mushroom.getPosition().x < 0 && m_increment.x < 0)) {
        m_increment.x = -m_increment.x;
    }

    if ((m_mushroom.getPosition().y > l_windSize.y - l_textSize.y && m_increment.y > 0)
        || (m_mushroom.getPosition().y < 0 && m_increment.y < 0)) {
        m_increment.y = -m_increment.y;
    }

    float fElapsed = m_elapsed.asSeconds();

    m_mushroom.setPosition(
            {m_mushroom.getPosition().x + (m_increment.x * fElapsed),
             m_mushroom.getPosition().y + (m_increment.y * fElapsed)});
}

///
void GameOfLife::render() {
    m_window.beginDraw();  // Clear.
    m_window.draw(m_mushroom);
    m_window.endDraw();  // Display.
}