#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

/**
 * @class PrimitiveRenderer
 * @brief Klasa odpowiedzialna za rysowanie prymitywów 2D na sf::RenderTexture.
 *
 * Obsługuje rysowanie:
 * - punktów,
 * - linii (DDA i domyślną),
 * - okręgów i elips,
 * - łamanych otwartych i zamkniętych,
 * - wypełnień metodą flood fill i boundary fill.
 */
class PrimitiveRenderer {
private:
    sf::RenderTexture& canvas; ///< Referencja do tekstury, na której rysujemy.

public:
    /**
     * @brief Konstruktor z przypisaniem referencji do canvas.
     * @param canvas Tekstura renderująca.
     */
    explicit PrimitiveRenderer(sf::RenderTexture& canvas)
        : canvas(canvas) {
    }

    /**
     * @brief Rysuje pojedynczy punkt.
     * @param position Pozycja punktu.
     * @param color Kolor punktu.
     */
    void drawPoint(const sf::Vector2f& position, sf::Color color);

    /**
     * @brief Wypełnia obszar kolorem metodą flood fill.
     * @param P Punkt startowy wypełnienia.
     * @param fill_color Kolor wypełnienia.
     * @param background_color Kolor tła (granicy wypełnienia).
     */
    void flood_fill(const sf::Vector2f& P, sf::Color fill_color, sf::Color background_color);

    /**
     * @brief Wypełnia obszar kolorem metodą boundary fill.
     * @param P Punkt startowy wypełnienia.
     * @param fill_color Kolor wypełnienia.
     * @param boundry_color Kolor granicy, której nie należy przekraczać.
     */
    void boundry_fill(const sf::Vector2f& P, sf::Color fill_color, sf::Color boundry_color);

    /**
     * @brief Rysuje linię metodą przyrostową (DDA).
     * @param start Punkt początkowy.
     * @param end Punkt końcowy.
     * @param color Kolor linii.
     */
    void drawLine(const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color);

    /**
     * @brief Rysuje linię przy użyciu domyślnego sf::Vertex.
     * @param start Punkt początkowy.
     * @param end Punkt końcowy.
     * @param color Kolor linii.
     */
    void drawLineDom(const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color);

    /**
     * @brief Rysuje okrąg i wypełnia go kolorem.
     * @param pos Środek okręgu.
     * @param R Promień okręgu.
     * @param color Kolor konturu.
     * @param fill Kolor wypełnienia.
     */
    void drawCircle(const sf::Vector2f& pos, const float R, sf::Color color, sf::Color fill);

    /**
     * @brief Rysuje elipsę i wypełnia ją kolorem.
     * @param pos Środek elipsy.
     * @param Rx Promień w osi X.
     * @param Ry Promień w osi Y.
     * @param color Kolor konturu.
     * @param fill Kolor wypełnienia.
     */
    void drawElips(const sf::Vector2f& pos, const float Rx, const float Ry, sf::Color color, sf::Color fill);

    /**
     * @brief Rysuje łamaną otwartą z podanych punktów.
     * @param points Wektor punktów łamanej.
     * @param color Kolor łamanej.
     */
    void drawPolyline(const std::vector<sf::Vector2f>& points, sf::Color color);

    /**
     * @brief Rysuje łamaną zamkniętą (poligon).
     * @param points Wektor punktów poligonu.
     * @param color Kolor poligonu.
     */
    void drawPolygon(const std::vector<sf::Vector2f>& points, sf::Color color);

    /**
     * @brief Zwraca referencję do tekstury renderującej.
     * @return Referencja do sf::RenderTexture.
     */
    sf::RenderTexture& getCanvas() { return canvas; }
};
