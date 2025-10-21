#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class PrimitiveRenderer {
private:
    sf::RenderWindow* window;

public:
    explicit PrimitiveRenderer(sf::RenderWindow* targetWindow)
        : window(targetWindow) {
    }

    void drawPoint(const sf::Vector2f& position, sf::Color color);

    void drawLine(const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color);

    void drawLineDom(const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color);

    // --- Łamana otwarta (zbiór punktów) ---
    void drawPolyline(const std::vector<sf::Vector2f>& points, sf::Color color);

    // --- Łamana zamknięta (poligon) ---
    void drawPolygon(const std::vector<sf::Vector2f>& points, sf::Color color);
};