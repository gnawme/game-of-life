/// \file Window.cpp
/// \author Norm Evangelista
/// \copyright (c) 2025 Norm Evangelista
// Copyright 2025 gnawme (Norm Evangelista)
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
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/WindowEnums.hpp"
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

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

    // Try multiple OpenGL context configurations in order of preference
    std::vector<sf::ContextSettings> settingsToTry;

    // Option 1: Let SFML choose (original behavior)
    settingsToTry.push_back(sf::ContextSettings());

    // Option 2: OpenGL 2.1 (maximum compatibility)
    sf::ContextSettings settings21;
    settings21.majorVersion = 2;
    settings21.minorVersion = 1;
    settings21.attributeFlags = sf::ContextSettings::Default;
    settingsToTry.push_back(settings21);

    // Option 3: OpenGL 3.0 compatibility profile
    sf::ContextSettings settings30;
    settings30.majorVersion = 3;
    settings30.minorVersion = 0;
    settings30.attributeFlags = sf::ContextSettings::Default;
    settingsToTry.push_back(settings30);

    // Try each configuration
    bool created = false;
    for (const auto& settings : settingsToTry) {
        try {
            m_window.create(
                    sf::VideoMode({m_windowSize.x, m_windowSize.y}),
                    m_windowTitle,
                    state,
                    settings);
            if (m_window.isOpen()) {
                created = true;
                break;
            }
        } catch (...) {
            // Try next configuration
            continue;
        }
    }

    if (!created) {
        throw std::runtime_error("Failed to create SFML window with any OpenGL configuration");
    }

    // Center the window on screen (skip if fullscreen)
    if (!m_isFullscreen) {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        int x = (desktop.size.x - m_windowSize.x) / 2;
        int y = (desktop.size.y - m_windowSize.y) / 2;
        m_window.setPosition(sf::Vector2i(x, y));
    }
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