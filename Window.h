/// \file Window.h
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
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window.hpp>

#include <string>

namespace gol {
class Window {
public:
    Window();
    Window(std::string& title, const sf::Vector2u& size);
    ~Window();

    void beginDraw();
    void endDraw();

    void update();

    bool isDone() const;
    bool isFullscreen() const;
    sf::Vector2u getWindowSize();

    void toggleFullScreen();

    void draw(sf::Drawable& l_drawable);

private:
    void create();
    void destroy();

    sf::RenderWindow m_window;
    sf::Vector2u m_windowSize{640, 640};
    sf::String m_windowTitle{"Window"};
    bool m_isDone{false};
    bool m_isFullscreen{false};
};
}  // namespace gol