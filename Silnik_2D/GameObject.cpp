#include "GameObject.hpp"

// ------------------------------
// ShapeObject
// ------------------------------

// Konstruktor bazowy dla figur
ShapeObject::ShapeObject(sf::Color c) : color(c) {}

// ------------------------------
// Point
// ------------------------------

// Konstruktor punktu
Point::Point(float x, float y, sf::Color c)
    : ShapeObject(c), position(x, y) {
}

// Rysowanie punktu na kanwie
void Point::draw(PrimitiveRenderer& renderer) {
    renderer.drawPoint(position, color);
}

// Przesunięcie punktu o tx, ty
void Point::translate(float tx, float ty) {
    position.x += tx;
    position.y -= ty; // y odwrócone względem układu współrzędnych SFML
}

// Obrót punktu wokół zadanego centrum
void Point::rotate(float angleDeg, const sf::Vector2f& center) {
    float rad = angleDeg * 3.14159265f / 180.f;
    float x1 = position.x, y1 = position.y;
    position.x = center.x + (x1 - center.x) * std::cos(rad) - (y1 - center.y) * std::sin(rad);
    position.y = center.y + (x1 - center.x) * std::sin(rad) + (y1 - center.y) * std::cos(rad);
}

// Skalowanie punktu względem centrum
void Point::scale(float kx, float ky, const sf::Vector2f& center) {
    position.x = center.x + (position.x - center.x) * kx;
    position.y = center.y + (position.y - center.y) * ky;
}

// Getter pozycji punktu
sf::Vector2f Point::getPosition() const { return position; }

// ------------------------------
// Line
// ------------------------------

// Konstruktor linii z dwóch punktów
Line::Line(const sf::Vector2f& a, const sf::Vector2f& b, sf::Color c)
    : ShapeObject(c), start(a.x, a.y, c), end(b.x, b.y, c) {
}

// Rysowanie linii
void Line::draw(PrimitiveRenderer& renderer) {
    renderer.drawLine(start.getPosition(), end.getPosition(), color);
}

// Przesunięcie linii
void Line::translate(float tx, float ty) {
    start.translate(tx, ty);
    end.translate(tx, ty);
}

// Obrót linii wokół centrum
void Line::rotate(float angleDeg, const sf::Vector2f& center) {
    start.rotate(angleDeg, center);
    end.rotate(angleDeg, center);
}

// Skalowanie linii
void Line::scale(float kx, float ky, const sf::Vector2f& center) {
    start.scale(kx, ky, center);
    end.scale(kx, ky, center);
}

// ------------------------------
// CircleShapeObject
// ------------------------------

// Konstruktor koła
CircleShapeObject::CircleShapeObject(const sf::Vector2f& c, float r, sf::Color col)
    : ShapeObject(col), center(c), radius(r) {
}

// Rysowanie koła
void CircleShapeObject::draw(PrimitiveRenderer& renderer) {
    renderer.drawCircle(center, radius, color, color);
}

// Przesunięcie koła
void CircleShapeObject::translate(float tx, float ty) {
    center.x += tx;
    center.y -= ty;
}

// Obrót koła wokół punktu
void CircleShapeObject::rotate(float angleDeg, const sf::Vector2f& rotationCenter) {
    float rad = angleDeg * 3.14159265f / 180.f;
    float x1 = center.x, y1 = center.y;
    center.x = rotationCenter.x + (x1 - rotationCenter.x) * std::cos(rad) - (y1 - rotationCenter.y) * std::sin(rad);
    center.y = rotationCenter.y + (x1 - rotationCenter.x) * std::sin(rad) + (y1 - rotationCenter.y) * std::cos(rad);
}

// Skalowanie koła (średnica zależna od średniej kx i ky)
void CircleShapeObject::scale(float kx, float ky, const sf::Vector2f& scaleCenter) {
    center.x = scaleCenter.x + (center.x - scaleCenter.x) * kx;
    center.y = scaleCenter.y + (center.y - scaleCenter.y) * ky;
    radius *= (kx + ky) / 2.f;
}

// ------------------------------
// Okreg — ruchome koło z logiką
// ------------------------------

Okreg::Okreg(sf::Vector2f c, float r, sf::Color col)
    : center(c), radius(r), color(col), angularSpeed(0.f), moveSpeed(0.f), moveAngle(0.f) {
}

// Ustawienie ruchu liniowego
void Okreg::setMovement(float speed, float angleRad) {
    moveSpeed = speed;
    moveAngle = angleRad;
}

// Ustawienie prędkości obrotu
void Okreg::setRotation(float degPerSec) {
    angularSpeed = degPerSec;
}

// Aktualizacja pozycji i obrotu
void Okreg::update(float dt) {
    if (moveSpeed != 0.f) {
        float dx = moveSpeed * std::cos(moveAngle) * dt;
        float dy = moveSpeed * std::sin(moveAngle) * dt;
        translate(dx, dy);
    }

    if (angularSpeed != 0.f) {
        rotate(angularSpeed * dt, center);
    }
}

// Rysowanie okręgu
void Okreg::draw(PrimitiveRenderer& renderer) {
    renderer.drawCircle(center, radius, color, color);
}

// Przesunięcie okręgu
void Okreg::translate(float tx, float ty) {
    center.x += tx;
    center.y -= ty;
}

// Obrót okręgu wokół punktu
void Okreg::rotate(float angleDeg, const sf::Vector2f& pivot) {
    float rad = angleDeg * 3.14159265f / 180.f;
    float x2 = pivot.x + (center.x - pivot.x) * std::cos(rad) - (center.y - pivot.y) * std::sin(rad);
    float y2 = pivot.y + (center.x - pivot.x) * std::sin(rad) + (center.y - pivot.y) * std::cos(rad);
    center = { x2, y2 };
}

// Skalowanie okręgu
void Okreg::scale(float kx, float ky, const sf::Vector2f& pivot) {
    center.x = pivot.x + (center.x - pivot.x) * kx;
    center.y = pivot.y + (center.y - pivot.y) * ky;
    radius *= (kx + ky) / 2.f;
}

// ------------------------------
// BitmapObject — obsługa bitmap
// ------------------------------

BitmapObject::BitmapObject(const std::vector<BitmapHandler>& bmps)
    : bitmaps(bmps)
{
    if (bitmaps.empty() || !bitmaps[0].getTexture())
        throw std::runtime_error("BitmapObject: No valid bitmap loaded");

    auto texPtr = bitmaps[0].getTexture();
    sprite = std::make_unique<sf::Sprite>(*texPtr);

    // Ustawienie środka sprite'a na środek bitmapy
    sprite->setOrigin(sf::Vector2f{ texPtr->getSize().x / 2.f, texPtr->getSize().y / 2.f });
}

// Rysowanie bitmapy
void BitmapObject::draw(PrimitiveRenderer& renderer) {
    if (sprite) renderer.getCanvas().draw(*sprite);
}

// Przesunięcie bitmapy
void BitmapObject::translate(float tx, float ty) {
    if (sprite) sprite->move(sf::Vector2f{ tx, ty });
}

// Obrót bitmapy wokół punktu
void BitmapObject::rotate(float angleDeg, const sf::Vector2f& center) {
    if (!sprite) return;
    sprite->setOrigin(center);
    sf::Angle a = sf::degrees(angleDeg);
    sprite->rotate(a);
}

// Skalowanie bitmapy
void BitmapObject::scale(float kx, float ky, const sf::Vector2f& center) {
    if (!sprite) return;
    sprite->setOrigin(center);
    sprite->scale(sf::Vector2f{ kx, ky });
}

// ------------------------------
// SpriteObject — animowana bitmapa
// ------------------------------

SpriteObject::SpriteObject(const std::vector<BitmapHandler>& bmps, int frameCnt)
    : BitmapObject(bmps), frameCount(frameCnt)
{
    if (frameCount > bitmaps.size()) frameCount = bitmaps.size();
}

// Aktualizacja animacji
void SpriteObject::animate(float dt) {
    animTimer += dt;
    if (animTimer >= frameTime) {
        animTimer = 0.f;
        frame = (frame + 1) % frameCount;
        sprite->setTexture(*bitmaps[frame].getTexture());
    }
}

// Aktualizacja (wywołuje animację)
void SpriteObject::update(float dt) {
    animate(dt);
}

// ------------------------------
// Player — gracz sterowany klawiaturą
// ------------------------------

Player::Player(const std::vector<std::vector<BitmapHandler>>& bmps)
    : SpriteObject(bmps[DOWN], bmps[DOWN].size()), directionalBitmaps(bmps)
{
    auto texPtr = directionalBitmaps[DOWN][0].getTexture();
    if (!texPtr)
        throw std::runtime_error("Player: no valid texture for DOWN direction");

    sprite->setOrigin(sf::Vector2f{ texPtr->getSize().x / 2.f, texPtr->getSize().y / 2.f });
}

// Obsługa ruchu gracza z klawiatury
void Player::handleKeyboard(float dt) {
    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { movement.y -= speed * dt; dir = UP; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) { movement.y += speed * dt; dir = DOWN; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) { movement.x -= speed * dt; dir = LEFT; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) { movement.x += speed * dt; dir = RIGHT; }

    sprite->move(movement);
}

// Animacja gracza (zależna od kierunku i ruchu)
void Player::animate(float dt) {
    animTimer += dt;

    bool moving = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

    if (!moving) frame = 0; // Brak ruchu, pierwsza klatka
    else if (animTimer >= frameTime) {
        animTimer = 0.f;
        frame = (frame + 1) % directionalBitmaps[dir].size();
    }

    sprite->setTexture(*directionalBitmaps[dir][frame].getTexture());
}

// Aktualizacja stanu gracza
void Player::update(float dt) {
    handleKeyboard(dt);
    animate(dt);
}
