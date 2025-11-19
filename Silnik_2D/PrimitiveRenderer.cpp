#include "PrimitiveRenderer.hpp"
#include <cmath>
#include <stack>

// ------------------------------
// Rysowanie pojedynczego punktu
// ------------------------------
void PrimitiveRenderer::drawPoint(const sf::Vector2f& position, sf::Color color) {
    // Zaokrąglenie pozycji do najbliższej pikselowej
    sf::Vertex vertex{ sf::Vector2f(std::round(position.x), std::round(position.y)), color };
    canvas.draw(&vertex, 1, sf::PrimitiveType::Points);
}

// ------------------------------
// Algorytm wypełniania kolorem (flood fill)
// ------------------------------
void PrimitiveRenderer::flood_fill(const sf::Vector2f& P, sf::Color fill_color, sf::Color background_color) {
    sf::Vector2u size = canvas.getSize();
    if (P.x < 0 || P.y < 0 || P.x >= size.x || P.y >= size.y)
        return;

    sf::Image image = canvas.getTexture().copyToImage();
    const unsigned int width = size.x;
    const unsigned int height = size.y;

    sf::Color startColor = image.getPixel(sf::Vector2u{ (unsigned int)P.x, (unsigned int)P.y });
    if (startColor == fill_color || startColor != background_color)
        return;

    // Stos do iteracyjnego wypełniania
    std::stack<sf::Vector2u> stack;
    stack.push({ (unsigned int)P.x, (unsigned int)P.y });

    while (!stack.empty()) {
        auto p = stack.top();
        stack.pop();

        if (p.x >= width || p.y >= height)
            continue;

        sf::Color current = image.getPixel(p);
        if (current == background_color) {
            image.setPixel(sf::Vector2u{ p.x, p.y }, fill_color);

            // Dodanie sąsiadów do stosu
            if (p.x > 0) stack.push({ p.x - 1, p.y });
            if (p.x + 1 < width) stack.push({ p.x + 1, p.y });
            if (p.y > 0) stack.push({ p.x, p.y - 1 });
            if (p.y + 1 < height) stack.push({ p.x, p.y + 1 });
        }
    }

    // Aktualizacja tekstury i rysowanie
    sf::Texture newTexture;
    newTexture.loadFromImage(image);
    sf::Sprite sprite(newTexture);
    canvas.draw(sprite);
}

// ------------------------------
// Algorytm wypełniania kolorem z granicą (boundary fill)
// ------------------------------
void PrimitiveRenderer::boundry_fill(const sf::Vector2f& P, sf::Color fill_color, sf::Color boundry_color) {
    sf::Vector2u size = canvas.getSize();
    if (P.x < 0 || P.y < 0 || P.x >= size.x || P.y >= size.y)
        return;

    sf::Image image = canvas.getTexture().copyToImage();
    const unsigned int width = size.x;
    const unsigned int height = size.y;

    std::stack<sf::Vector2u> stack;
    stack.push({ (unsigned int)P.x, (unsigned int)P.y });

    while (!stack.empty()) {
        auto p = stack.top();
        stack.pop();

        if (p.x >= width || p.y >= height)
            continue;

        sf::Color current = image.getPixel(p);
        if (current != boundry_color && current != fill_color) {
            image.setPixel(sf::Vector2u{ p.x, p.y }, fill_color);

            if (p.x > 0) stack.push({ p.x - 1, p.y });
            if (p.x + 1 < width) stack.push({ p.x + 1, p.y });
            if (p.y > 0) stack.push({ p.x, p.y - 1 });
            if (p.y + 1 < height) stack.push({ p.x, p.y + 1 });
        }
    }

    sf::Texture newTexture;
    newTexture.loadFromImage(image);
    sf::Sprite sprite(newTexture);
    canvas.draw(sprite);
}

// ------------------------------
// Rysowanie linii metodą inkrementacyjną
// ------------------------------
void PrimitiveRenderer::drawLine(const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color) {
    float dx = end.x - start.x;
    float dy = end.y - start.y;

    float steps = std::max(std::abs(dx), std::abs(dy));
    if (steps == 0) {
        drawPoint(start, color);
        return;
    }

    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = start.x;
    float y = start.y;
    for (int i = 0; i <= static_cast<int>(steps); ++i) {
        drawPoint({ x, y }, color);
        x += xInc;
        y += yInc;
    }
}

// ------------------------------
// Rysowanie linii bezpośrednio (SFML)
// ------------------------------
void PrimitiveRenderer::drawLineDom(const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color) {
    sf::Vertex line[] = {
        sf::Vertex{start, color},
        sf::Vertex{end, color}
    };
    canvas.draw(line, 2, sf::PrimitiveType::Lines);
}

// ------------------------------
// Rysowanie okręgu i wypełnienie
// ------------------------------
void PrimitiveRenderer::drawCircle(const sf::Vector2f& pos, float R, sf::Color color, sf::Color fill) {
    float a = 0;
    while (a <= 45) { // Rysowanie okręgu w 8 symetrycznych ćwiartkach
        float cosA = std::cos(a);
        float sinA = std::sin(a);

        drawPoint({ pos.x + R * cosA, pos.y + R * sinA }, color);
        drawPoint({ pos.x - R * cosA, pos.y + R * sinA }, color);
        drawPoint({ pos.x + R * cosA, pos.y - R * sinA }, color);
        drawPoint({ pos.x - R * cosA, pos.y - R * sinA }, color);

        drawPoint({ pos.x + R * sinA, pos.y + R * cosA }, color);
        drawPoint({ pos.x - R * sinA, pos.y + R * cosA }, color);
        drawPoint({ pos.x + R * sinA, pos.y - R * cosA }, color);
        drawPoint({ pos.x - R * sinA, pos.y - R * cosA }, color);

        a += 0.1f;
    }
    boundry_fill(pos, fill, color); // wypełnienie wnętrza kolorem
}

// ------------------------------
// Rysowanie elipsy i wypełnienie
// ------------------------------
void PrimitiveRenderer::drawElips(const sf::Vector2f& pos, float Rx, float Ry, sf::Color color, sf::Color fill) {
    float a = 0;
    while (a <= 45) { // Rysowanie w 8 symetrycznych ćwiartkach
        float cosA = std::cos(a);
        float sinA = std::sin(a);

        drawPoint({ pos.x + Rx * cosA, pos.y + Ry * sinA }, color);
        drawPoint({ pos.x - Rx * cosA, pos.y + Ry * sinA }, color);
        drawPoint({ pos.x + Rx * cosA, pos.y - Ry * sinA }, color);
        drawPoint({ pos.x - Rx * cosA, pos.y - Ry * sinA }, color);

        drawPoint({ pos.x + Rx * sinA, pos.y + Ry * cosA }, color);
        drawPoint({ pos.x - Rx * sinA, pos.y + Ry * cosA }, color);
        drawPoint({ pos.x + Rx * sinA, pos.y - Ry * cosA }, color);
        drawPoint({ pos.x - Rx * sinA, pos.y - Ry * cosA }, color);

        a += 0.1f;
    }
    boundry_fill(pos, fill, color); // wypełnienie wnętrza elipsy
}

// ------------------------------
// Rysowanie łamanej (polilinii)
// ------------------------------
void PrimitiveRenderer::drawPolyline(const std::vector<sf::Vector2f>& points, sf::Color color) {
    if (points.size() < 2) return;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        drawLine(points[i], points[i + 1], color);
    }
}

// ------------------------------
// Rysowanie poligonu (zamkniętej łamanej)
// ------------------------------
void PrimitiveRenderer::drawPolygon(const std::vector<sf::Vector2f>& points, sf::Color color) {
    if (points.size() < 2) return;

    drawPolyline(points, color);              // rysowanie kolejnych segmentów
    drawLine(points.back(), points.front(), color); // zamknięcie poligonu
}
