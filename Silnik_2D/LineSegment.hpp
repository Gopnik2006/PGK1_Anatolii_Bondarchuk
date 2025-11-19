#pragma once
#include "Point2D.hpp"

/**
 * @enum LineAlgorithm
 * @brief Algorytmy rysowania linii.
 *
 * - Default: używa metody domyślnej (sf::Vertex Line).
 * - Incremental: używa własnej metody przyrostowej (DDA).
 */
enum class LineAlgorithm { Default, Incremental };

/**
 * @class LineSegment
 * @brief Reprezentuje odcinek 2D między dwoma punktami.
 *
 * Obsługuje:
 * - przechowywanie punktów końcowych,
 * - kolor odcinka,
 * - rysowanie odcinka przy użyciu różnorodnych algorytmów.
 */
class LineSegment {
private:
    Point2D start;   ///< Punkt początkowy odcinka.
    Point2D end;     ///< Punkt końcowy odcinka.
    sf::Color color; ///< Kolor odcinka.

public:
    /**
     * @brief Konstruktor odcinka.
     * @param s Punkt początkowy.
     * @param e Punkt końcowy.
     * @param c Kolor odcinka (domyślnie biały).
     */
    LineSegment(
        const sf::Vector2f& s = { 0.f, 0.f },
        const sf::Vector2f& e = { 0.f, 0.f },
        sf::Color c = sf::Color::White)
        : start(s), end(e), color(c) {
    }

    // --- Gettery ---

    /**
     * @brief Zwraca punkt początkowy odcinka.
     * @return Punkt początkowy.
     */
    Point2D getStart() const { return start; }

    /**
     * @brief Zwraca punkt końcowy odcinka.
     * @return Punkt końcowy.
     */
    Point2D getEnd() const { return end; }

    float getStartX() const { return start.getX(); } ///< Zwraca X punktu startowego.
    float getStartY() const { return start.getY(); } ///< Zwraca Y punktu startowego.
    float getEndX() const { return end.getX(); }     ///< Zwraca X punktu końcowego.
    float getEndY() const { return end.getY(); }     ///< Zwraca Y punktu końcowego.

    /**
     * @brief Zwraca kolor odcinka.
     * @return Kolor odcinka.
     */
    sf::Color getColor() const { return color; }

    // --- Settery ---

    /**
     * @brief Ustawia punkt początkowy odcinka.
     * @param newStart Nowy punkt startowy.
     */
    void setStart(const Point2D& newStart) { start = newStart; }

    /**
     * @brief Ustawia punkt końcowy odcinka.
     * @param newEnd Nowy punkt końcowy.
     */
    void setEnd(const Point2D& newEnd) { end = newEnd; }

    /**
     * @brief Ustawia punkt początkowy odcinka.
     * @param x Współrzędna X punktu startowego.
     * @param y Współrzędna Y punktu startowego.
     */
    void setStart(float x, float y) { start.set(x, y); }

    /**
     * @brief Ustawia punkt końcowy odcinka.
     * @param x Współrzędna X punktu końcowego.
     * @param y Współrzędna Y punktu końcowego.
     */
    void setEnd(float x, float y) { end.set(x, y); }

    /**
     * @brief Ustawia kolor odcinka.
     * @param newColor Nowy kolor.
     */
    void setColor(sf::Color newColor) { color = newColor; }

    // --- Rysowanie ---

    /**
     * @brief Rysuje odcinek na zadanym rendererze.
     * @param renderer Obiekt odpowiedzialny za rysowanie prymitywów.
     * @param algorithm Algorytm rysowania linii (domyślnie Default).
     */
    void draw(PrimitiveRenderer& renderer, LineAlgorithm algorithm = LineAlgorithm::Default) const {
        if (algorithm == LineAlgorithm::Incremental)
            renderer.drawLine(start.get(), end.get(), color);
        else
            renderer.drawLineDom(start.get(), end.get(), color);
    }
};
