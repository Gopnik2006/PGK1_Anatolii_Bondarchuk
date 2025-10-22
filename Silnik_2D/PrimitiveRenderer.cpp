#include "PrimitiveRenderer.hpp"
#include <cmath>


void PrimitiveRenderer::drawPoint(const sf::Vector2f& position, sf::Color color) {
    sf::Vertex vertex{ sf::Vector2f(round(position.x), round(position.y)), color };
    window->draw(&vertex, 1, sf::PrimitiveType::Points);
}

void PrimitiveRenderer::drawLine(const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color) {
    float dx = end.x - start.x;
    float dy = end.y - start.y;

    float kroki = std::max(std::abs(dx), std::abs(dy));
    if (kroki == 0) { // długość = 0
        drawPoint(start, color);
        return;
    }

    float xInc = dx / kroki;
    float yInc = dy / kroki;

    float x = start.x;
    float y = start.y;
    for (int i = 0; i <= static_cast<int>(kroki); ++i) { //wartość float konfliktuje z <=
        drawPoint({ x, y }, color);
        x += xInc;
        y += yInc;
    }
}

void PrimitiveRenderer::drawLineDom(const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color) {
    sf::Vertex line[] = {
    sf::Vertex{start, color},
    sf::Vertex{end, color}
    };
    window->draw(line, 2, sf::PrimitiveType::Lines);
}

void PrimitiveRenderer::drawPolyline(const std::vector<sf::Vector2f>&points, sf::Color color) {
    if (points.size() < 2) return;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        drawLine(points[i],points[i + 1],color);
        }
    }

void PrimitiveRenderer::drawPolygon(const std::vector<sf::Vector2f>&points, sf::Color color) {
    if (points.size() < 2) return;

    drawPolyline(points, color);
    drawLine(points.back(),points.front(),color);
    }


