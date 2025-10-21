#pragma once
#include "PrimitiveRenderer.hpp"

class Point2D {
private:
    sf::Vector2f position;
    sf::Color color;

public:
    // --- Konstruktor ---
    Point2D(const sf::Vector2f position, sf::Color color = sf::Color::White): position(position), color(color) {
    }

    // --- Gettery ---
    float getX() const { return position.x; }
    float getY() const { return position.y; }
    sf::Vector2f get() const { return position; }
    sf::Color getColor() const { return color; }

    // --- Settery ---
    void setX(float newX) { position.x = newX; }
    void setY(float newY) { position.y = newY; }
    void set(float newX, float newY) { position.x = newX; position.y = newY; }
    void setColor(sf::Color newColor) { color = newColor; }

    // --- Rysowanie punktu ---
    void draw(PrimitiveRenderer& renderer) const {
        renderer.drawPoint(position, color);
    }
};