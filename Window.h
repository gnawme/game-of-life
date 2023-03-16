/// \file Window.h
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <string>

namespace gol {
class Window {
public:
    Window();
    Window(std::string_view title, const sf::Vector2u& size);
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

    sf::RenderWindow m_window{};
    sf::Vector2u m_windowSize{640, 640};
    std::string m_windowTitle{"Window"};
    bool m_isDone{false};
    bool m_isFullscreen{false};
};
}  // namespace gol