#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "PrimitiveRenderer.hpp"
#include "BitmapHandler.hpp"
#include <memory>

// ---------------------------------------------------------
// Klasy bazowe
// ---------------------------------------------------------

/**
 * @class GameObject
 * @brief Bazowa klasa wszystkich obiektów gry.
 */
class GameObject {
public:
    virtual ~GameObject() = default;
};

/**
 * @class UpdatableObject
 * @brief Interfejs obiektów aktualizowanych co klatkę.
 */
class UpdatableObject : virtual public GameObject {
public:
    /**
     * @brief Aktualizacja logiki obiektu.
     * @param dt Czas w sekundach od poprzedniej klatki.
     */
    virtual void update(float dt) = 0;
};

/**
 * @class DrawableObject
 * @brief Interfejs obiektów, które mogą być rysowane.
 */
class DrawableObject : virtual public GameObject {
public:
    /**
     * @brief Rysuje obiekt przy użyciu PrimitiveRenderer.
     * @param renderer Obiekt odpowiedzialny za rysowanie prymitywów.
     */
    virtual void draw(PrimitiveRenderer& renderer) = 0;
};

/**
 * @class AnimatedObject
 * @brief Interfejs obiektów animowanych.
 */
class AnimatedObject : virtual public UpdatableObject {
public:
    /**
     * @brief Aktualizuje stan animacji.
     * @param dt Czas w sekundach od poprzedniej klatki.
     */
    virtual void animate(float dt) = 0;
};

/**
 * @class TransformableObject
 * @brief Interfejs obiektów podlegających transformacjom.
 */
class TransformableObject : virtual public GameObject {
public:
    /**
     * @brief Przesuwa obiekt.
     * @param tx Przesunięcie po osi X.
     * @param ty Przesunięcie po osi Y.
     */
    virtual void translate(float tx, float ty) = 0;

    /**
     * @brief Obraca obiekt o zadany kąt.
     * @param angleDeg Kąt w stopniach.
     * @param center Punkt obrotu.
     */
    virtual void rotate(float angleDeg, const sf::Vector2f& center = { 0.f, 0.f }) = 0;

    /**
     * @brief Skaluje obiekt.
     * @param kx Skala w osi X.
     * @param ky Skala w osi Y.
     * @param center Punkt skalowania.
     */
    virtual void scale(float kx, float ky, const sf::Vector2f& center = { 0.f, 0.f }) = 0;
};

// ---------------------------------------------------------
// ShapeObject — klasa bazowa dla figur
// ---------------------------------------------------------

/**
 * @class ShapeObject
 * @brief Bazowa klasa dla obiektów geometrycznych.
 */
class ShapeObject : public DrawableObject, public TransformableObject {
protected:
    sf::Color color; ///< Kolor obiektu.

public:
    /**
     * @brief Konstruktor ustawiający kolor figury.
     * @param c Kolor obiektu.
     */
    ShapeObject(sf::Color c = sf::Color::White);
};

// ---------------------------------------------------------
// Klasy pochodne — obiekty geometryczne
// ---------------------------------------------------------

/**
 * @class Point
 * @brief Reprezentuje punkt 2D.
 */
class Point : public ShapeObject {
private:
    sf::Vector2f position; ///< Pozycja punktu.

public:
    /**
     * @brief Konstruktor punktu.
     * @param x Pozycja X.
     * @param y Pozycja Y.
     * @param c Kolor punktu.
     */
    Point(float x, float y, sf::Color c = sf::Color::White);

    void draw(PrimitiveRenderer& renderer) override;
    void translate(float tx, float ty) override;
    void rotate(float angleDeg, const sf::Vector2f& center = { 0.f, 0.f }) override;
    void scale(float kx, float ky, const sf::Vector2f& center = { 0.f, 0.f }) override;

    /**
     * @brief Zwraca pozycję punktu.
     * @return Wektor pozycji.
     */
    sf::Vector2f getPosition() const;
};

/**
 * @class Line
 * @brief Reprezentuje odcinek między dwoma punktami.
 */
class Line : public ShapeObject {
private:
    Point start; ///< Punkt początkowy.
    Point end;   ///< Punkt końcowy.

public:
    /**
     * @brief Konstruktor odcinka.
     * @param a Punkt startowy.
     * @param b Punkt końcowy.
     * @param c Kolor odcinka.
     */
    Line(const sf::Vector2f& a, const sf::Vector2f& b, sf::Color c = sf::Color::White);

    void draw(PrimitiveRenderer& renderer) override;
    void translate(float tx, float ty) override;
    void rotate(float angleDeg, const sf::Vector2f& center = { 0.f, 0.f }) override;
    void scale(float kx, float ky, const sf::Vector2f& center = { 0.f, 0.f }) override;
};

/**
 * @class CircleShapeObject
 * @brief Reprezentuje okrąg 2D.
 */
class CircleShapeObject : public ShapeObject {
private:
    sf::Vector2f center; ///< Środek okręgu.
    float radius;        ///< Promień.

public:
    /**
     * @brief Konstruktor okręgu.
     * @param c Pozycja środka.
     * @param r Promień.
     * @param col Kolor.
     */
    CircleShapeObject(const sf::Vector2f& c, float r, sf::Color col = sf::Color::White);

    void draw(PrimitiveRenderer& renderer) override;
    void translate(float tx, float ty) override;
    void rotate(float angleDeg, const sf::Vector2f& rotationCenter = { 0.f, 0.f }) override;
    void scale(float kx, float ky, const sf::Vector2f& scaleCenter = { 0.f, 0.f }) override;
};

// ---------------------------------------------------------
// Obiekty z logiką — animacje, ruch, itp.
// ---------------------------------------------------------

/**
 * @class Okreg
 * @brief Ruchomy okrąg posiadający animację oraz logikę ruchu.
 */
class Okreg : public ShapeObject, public UpdatableObject {
private:
    sf::Vector2f center; ///< Środek okręgu.
    float radius;        ///< Promień.
    sf::Color color;     ///< Kolor okręgu.
    float angularSpeed;  ///< Prędkość obrotowa (deg/s).
    float moveSpeed;     ///< Prędkość liniowa.
    float moveAngle;     ///< Kierunek ruchu w radianach.

public:
    /**
     * @brief Konstruktor ruchomego okręgu.
     * @param c Pozycja środka.
     * @param r Promień.
     * @param col Kolor okręgu.
     */
    Okreg(sf::Vector2f c, float r, sf::Color col = sf::Color::Green);

    /**
     * @brief Ustawia ruch liniowy.
     * @param speed Prędkość.
     * @param angleRad Kierunek w radianach.
     */
    void setMovement(float speed, float angleRad);

    /**
     * @brief Ustawia prędkość obrotową.
     * @param degPerSec Stopnie na sekundę.
     */
    void setRotation(float degPerSec);

    void update(float dt) override;
    void draw(PrimitiveRenderer& renderer) override;
    void translate(float tx, float ty) override;
    void rotate(float angleDeg, const sf::Vector2f& pivot) override;
    void scale(float kx, float ky, const sf::Vector2f& pivot) override;
};

// ---------------------------------------------------------
// Bitmapy i obiekty animowane
// ---------------------------------------------------------

/**
 * @class BitmapObject
 * @brief Obiekt rysowany na podstawie bitmap.
 */
class BitmapObject : public DrawableObject, public TransformableObject {
protected:
    std::vector<BitmapHandler> bitmaps; ///< Lista bitmap.
    std::unique_ptr<sf::Sprite> sprite; ///< Główny sprite.

public:
    /**
     * @brief Domyślny konstruktor.
     */
    BitmapObject() = default;

    /**
     * @brief Konstruktor z listą bitmap.
     * @param bmps Wektor bitmap.
     */
    BitmapObject(const std::vector<BitmapHandler>& bmps);

    void draw(PrimitiveRenderer& renderer) override;
    void translate(float tx, float ty) override;
    void rotate(float angleDeg, const sf::Vector2f& center = { 0,0 }) override;
    void scale(float kx, float ky, const sf::Vector2f& center = { 0,0 }) override;
};

/**
 * @class SpriteObject
 * @brief Obiekt animowany z wieloma klatkami bitmap.
 */
class SpriteObject : public BitmapObject, public AnimatedObject {
protected:
    float frameTime = 0.12f; ///< Czas jednej klatki animacji.
    float animTimer = 0.f;   ///< Zegar animacji.
    int frame = 0;           ///< Aktualna klatka.
    int frameCount = 1;      ///< Liczba klatek.

public:
    /**
     * @brief Konstruktor sprite'a.
     * @param bmps Wektor bitmap.
     * @param frameCnt Ilość klatek animacji.
     */
    SpriteObject(const std::vector<BitmapHandler>& bmps, int frameCnt = 1);

    void animate(float dt) override;
    void update(float dt) override;
};

/**
 * @class Player
 * @brief Sterowalna postać animowana.
 */
class Player : public SpriteObject {
private:
    float speed = 120.f; ///< Prędkość ruchu gracza.

    /**
     * @brief Kierunki animacji gracza.
     */
    enum Direction { DOWN, UP, LEFT, RIGHT } dir = DOWN;

    std::vector<std::vector<BitmapHandler>> directionalBitmaps; ///< Bitmapy dla każdego kierunku i klatki.

public:
    /**
     * @brief Konstruktor gracza.
     * @param bmps Bitmapy dla wszystkich kierunków animacji.
     */
    Player(const std::vector<std::vector<BitmapHandler>>& bmps);

    /**
     * @brief Obsługuje klawiaturę i ruch gracza.
     * @param dt Czas od ostatniej klatki.
     */
    void handleKeyboard(float dt);

    void animate(float dt) override;
    void update(float dt) override;
};