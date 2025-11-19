#pragma once
#include "PrimitiveRenderer.hpp"

/**
 * @class Point2D
 * @brief Reprezentuje pojedynczy punkt 2D z kolorem.
 *
 * Obsługuje operacje:
 * - pobieranie i ustawianie pozycji,
 * - pobieranie i ustawianie koloru,
 * - przesuwanie punktu,
 * - rysowanie przy użyciu PrimitiveRenderer.
 */
class Point2D {
private:
    sf::Vector2f position; ///< Pozycja punktu w przestrzeni 2D.
    sf::Color color;       ///< Kolor punktu.

public:
    /**
     * @brief Konstruktor punktu.
     * @param position Pozycja punktu.
     * @param color Kolor punktu (domyślnie biały).
     */
    Point2D(const sf::Vector2f position, sf::Color color = sf::Color::White)
        : position(position), color(color) {
    }

    // --- Gettery ---

    /**
     * @brief Pobiera współrzędną X punktu.
     * @return Współrzędna X.
     */
    float getX() const { return position.x; }

    /**
     * @brief Pobiera współrzędną Y punktu.
     * @return Współrzędna Y.
     */
    float getY() const { return position.y; }

    /**
     * @brief Pobiera wektor pozycji punktu.
     * @return Wektor (x, y).
     */
    sf::Vector2f get() const { return position; }

    /**
     * @brief Pobiera kolor punktu.
     * @return Kolor punktu.
     */
    sf::Color getColor() const { return color; }

    // --- Settery ---

    /**
     * @brief Ustawia współrzędną X punktu.
     * @param newX Nowa współrzędna X.
     */
    void setX(float newX) { position.x = newX; }

    /**
     * @brief Ustawia współrzędną Y punktu.
     * @param newY Nowa współrzędna Y.
     */
    void setY(float newY) { position.y = newY; }

    /**
     * @brief Ustawia pozycję punktu.
     * @param newX Nowa współrzędna X.
     * @param newY Nowa współrzędna Y.
     */
    void set(float newX, float newY) { position.x = newX; position.y = newY; }

    /**
     * @brief Ustawia kolor punktu.
     * @param newColor Nowy kolor.
     */
    void setColor(sf::Color newColor) { color = newColor; }

    // --- Operacje graficzne ---

    /**
     * @brief Rysuje punkt na podanym rendererze.
     * @param renderer Obiekt PrimitiveRenderer do rysowania.
     */
    void draw(PrimitiveRenderer& renderer) const {
        renderer.drawPoint(position, color);
    }

    /**
     * @brief Przesuwa punkt o wektor m.
     * @param m Wektor przesunięcia (dx, dy).
     */
    void move(sf::Vector2f m) {
        position.x += m.x;
        position.y += m.y;
    }
};
