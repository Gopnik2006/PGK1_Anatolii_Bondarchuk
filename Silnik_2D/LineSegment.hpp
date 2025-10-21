#pragma once
#include "Point2D.hpp"

enum class LineAlgorithm { Default, Incremental };

class LineSegment {
private:
    Point2D start;
    Point2D end;
    sf::Color color;

public:
    // --- Konstruktor ---
    LineSegment(
        const sf::Vector2f& s = { 0.f, 0.f },
        const sf::Vector2f& e = { 0.f, 0.f },
        sf::Color c = sf::Color::White)
        : start(s), end(e), color(c) {
    }

    // --- Gettery ---
    Point2D getStart() const { return start; }
    Point2D getEnd() const { return end; }

    float getStartX() const { return start.getX(); }
    float getStartY() const { return start.getY(); }
    float getEndX() const { return end.getX(); }
    float getEndY() const { return end.getY(); }

    sf::Color getColor() const { return color; }

    // --- Settery ---
    void setStart(const Point2D& newStart) { start = newStart; }
    void setEnd(const Point2D& newEnd) { end = newEnd; }

    void setStart(float x, float y) { start.set(x, y); }
    void setEnd(float x, float y) { end.set(x, y); }

    void setColor(sf::Color newColor) { color = newColor; }

    // --- Rysowanie odcinka ---
    void draw(PrimitiveRenderer& renderer, LineAlgorithm algorithm = LineAlgorithm::Default) const {
        if (algorithm == LineAlgorithm::Incremental)
            renderer.drawLine(start.get(), end.get(), color);
        else
            renderer.drawLineDom(start.get(), end.get(), color);
        
    }
};