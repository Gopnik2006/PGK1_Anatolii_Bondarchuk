#include "PrimitiveRenderer.hpp"
#include <cmath>


void PrimitiveRenderer::drawPoint(const sf::Vector2f& position, sf::Color color) {
    sf::Vertex vertex{ sf::Vector2f(round(position.x), round(position.y)), color };
    window->draw(&vertex, 1, sf::PrimitiveType::Points);
}

void PrimitiveRenderer::drawLine(const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color) {
    sf::Vector2f a = start;
    sf::Vector2f b = end;

    if (b.x < a.x) {
        std::swap(a, b);
    }
    

    float dx = b.x - a.x;
    float dy = b.y - a.y;

    if (dx == 0) {
        int sy = (b.y > a.y ? 1 : -1);
        while( a.y != b.y) {
            drawPoint(a, color);
            a.y += sy;
        }
        drawPoint(b, color);
        return;
    }

    float m = dy / dx; // nachylenie

    if (m > 1 || m < -1) {
        m = 1 / m;
        while(a.y <= b.y) {
            drawPoint(a, color);
            a.x += m;
            a.y++;
        }
    }else {
        while(a.x <= b.x) {
            drawPoint(a, color);
            a.y += m;
            a.x++;
        }
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


