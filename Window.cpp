/// \file Window.cpp
#include "Window.h"

namespace gol {
///
Window::Window() {
    create();
}

///
Window::Window(std::string_view title, const sf::Vector2u& size)
    : m_windowTitle(title)
    , m_windowSize(size) {
    create();
}

///
Window::~Window() {
    destroy();
}

///
void Window::create() {
    auto style = (m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
    m_window.create(sf::VideoMode({m_windowSize.x, m_windowSize.y}), m_windowTitle, style);
}

///
void Window::destroy() {
    m_window.close();
}

///
void Window::beginDraw() {
    m_window.clear(sf::Color::Black);
}

///
void Window::endDraw() {
    m_window.display();
}

///
bool Window::isDone() const {
    return m_isDone;
}

///
bool Window::isFullscreen() const {
    return m_isFullscreen;
}

///
void Window::draw(sf::Drawable& drawable) {
    m_window.draw(drawable);
}

///
sf::Vector2u Window::getWindowSize() {
    return m_windowSize;
}

///
void Window::toggleFullScreen() {
    m_isFullscreen = !m_isFullscreen;
    destroy();
    create();
}

///
void Window::update() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_isDone = true;
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::F5) {
                toggleFullScreen();
            } else if (event.key.code == sf::Keyboard::Escape) {
                m_isDone = true;
            }
        }
    }
}
}  // namespace gol