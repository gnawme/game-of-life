/// \file Window.cpp
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
#include "Window.h"
#include "SFML/Window/Keyboard.hpp"

namespace gol {
///
Window::Window() {
    create();
}

///
Window::Window(std::string& title, const sf::Vector2u& size)
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
    auto state = (m_isFullscreen ? sf::State::Fullscreen : sf::State::Windowed);
    m_window.create(sf::VideoMode({m_windowSize.x, m_windowSize.y}), m_windowTitle, state);
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
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_isDone = true;
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::F5) {
                toggleFullScreen();
            } else if (keyPressed->code == sf::Keyboard::Key::Escape) {
                m_isDone = true;
            }
        }
    }
}
}  // namespace gol