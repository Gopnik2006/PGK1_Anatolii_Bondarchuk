#include "Engine.hpp"
#include "LineSegment.hpp"
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <fstream>
#include <variant>
#include <optional>
#include <Windows.h>

/**
 * @brief Singleton instancji silnika.
 */
Engine* Engine::instance = nullptr;

/**
 * @struct FillRequest
 * @brief Struktura reprezentująca żądanie wypełnienia kolorem.
 */
struct FillRequest {
    sf::Vector2f pos; ///< Punkt startowy wypełnienia
    sf::Color color;  ///< Kolor wypełnienia
};

bool waitingForColor = false; ///< Flaga oczekiwania na wybór koloru

// --- Globalne kontenery rysowanych obiektów ---
std::vector<FillRequest> wypewnenia;
std::vector<sf::Vector2f> punkty;
std::vector<std::pair<sf::Vector2f, sf::Vector2f>> linie;
std::vector<std::vector<sf::Vector2f>> polyliny;
std::vector<std::vector<sf::Vector2f>> polygony;
std::vector<sf::Vector2f> okregy;
std::vector<sf::Vector2f> elipsy;

/**
 * @brief Konstruktor silnika.
 * @param config Konfiguracja silnika (rozmiar okna, kolor tła, fps, tryb fullscreen)
 */
Engine::Engine(const EngineConfig& config)
    : clearColor(config.clearColor),
    staticCanvas({ config.width, config.height }),
    animatedCanvas({ config.width, config.height }),
    canvasSprite(staticCanvas.getTexture())
{
    sf::VideoMode mode;
    mode.size = { config.width, config.height };

    // Ukrycie konsoli
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);

    window = sf::RenderWindow(mode, config.windowTitle);
    window.setFramerateLimit(config.fps);

    staticCanvas.clear(clearColor);
    staticCanvas.display();
    animatedCanvas.clear(sf::Color::Transparent);
    animatedCanvas.display();
}

/**
 * @brief Wczytuje bitmapę do canvas.
 * @param filename Ścieżka do pliku bitmapy.
 * @return true jeśli wczytano poprawnie, false w przeciwnym wypadku.
 */
bool Engine::loadBitmapToCanvas(const std::string& filename)
{
    if (!bitmap.loadFromFile(filename))
        return false;

    staticCanvas.clear(clearColor);

    sf::Sprite spr(*bitmap.getTexture());
    staticCanvas.draw(spr);
    staticCanvas.display();

    canvasSprite = sf::Sprite(staticCanvas.getTexture());
    return true;
}

/**
 * @brief Zapisuje obecny stan canvas do pliku.
 * @param filename Nazwa pliku.
 * @return true jeśli zapisano poprawnie, false w przeciwnym wypadku.
 */
bool Engine::saveCanvasToFile(const std::string& filename)
{
    sf::Image img = staticCanvas.getTexture().copyToImage();
    return img.saveToFile(filename);
}

/**
 * @brief Tworzy pusty canvas o zadanym rozmiarze i kolorze.
 * @param w Szerokość canvas.
 * @param h Wysokość canvas.
 * @param c Kolor tła.
 */
void Engine::createBlankCanvas(unsigned w, unsigned h, sf::Color c)
{
    bitmap.create(w, h, c);

    staticCanvas.clear(c);
    sf::Sprite s(*bitmap.getTexture());
    staticCanvas.draw(s);
    staticCanvas.display();
    canvasSprite = sf::Sprite(staticCanvas.getTexture());
}

/**
 * @brief Zapisuje log do pliku engine.log
 * @param message Wiadomość do zapisania
 */
static void log(const std::string& message) {
    std::ofstream logFile("engine.log", std::ios::app);
    logFile << "[LOG] " << message << "\n";
}

/**
 * @brief Zamyka silnik i zwalnia zasoby.
 */
void Engine::shutdown() {
    log("Shutting down engine...");
    window.close();
    ::ShowWindow(::GetConsoleWindow(), SW_SHOW);

    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

/**
 * @brief Inicjalizacja silnika.
 */
void Engine::init() {
    isRunning = true;
    std::cout << "[Engine] Initialized\n";
}

/**
 * @brief Obsługuje wszystkie zdarzenia wejścia (mysz, klawiatura, zamknięcie okna).
 */
void Engine::handleInput() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            isRunning = false;
            window.close();
        }

        // Obsługa myszy
        if (const auto* MousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (MousePressed->button == sf::Mouse::Button(0)) {
                sf::Vector2f pos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                punkty.push_back(pos);
            }
        }

        // Obsługa klawiatury
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
            case sf::Keyboard::Key::Escape:
                isRunning = false;
                window.close();
                break;

            case sf::Keyboard::Key::Backspace:
                bitmap.clear();
                staticCanvas.clear(clearColor);
                window.clear();
                window.draw(sf::Sprite(staticCanvas.getTexture()));
                window.display();
                break;

            case sf::Keyboard::Key::Space:
            {
                sf::Vector2f pos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                linie.push_back({ pos, pos + sf::Vector2f(50, 50) });
                break;
            }

            case sf::Keyboard::Key::P:
                polygony.push_back(punkty);
                punkty.clear();
                break;

            case sf::Keyboard::Key::L:
                polyliny.push_back(punkty);
                punkty.clear();
                break;

            case sf::Keyboard::Key::O:
            {
                sf::Vector2f pos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                okregy.push_back(pos);
                break;
            }

            case sf::Keyboard::Key::E:
            {
                sf::Vector2f pos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                elipsy.push_back(pos);
                break;
            }

            case sf::Keyboard::Key::Num1: loadBitmapToCanvas("tlo.png"); break;
            case sf::Keyboard::Key::Num2: saveCanvasToFile("zrzut.png"); break;
            case sf::Keyboard::Key::Num3: createBlankCanvas(1280, 720, sf::Color::White); break;
            case sf::Keyboard::Key::Num4: loadBitmapToCanvas("zrzut.png"); break;

            case sf::Keyboard::Key::F:
                waitingForColor = true;
                std::cout << "Waiting for color key...\n";
                break;

            default:
                if (waitingForColor) {
                    sf::Color chosenColor;
                    switch (keyPressed->code) {
                    case sf::Keyboard::Key::R: chosenColor = sf::Color::Red; break;
                    case sf::Keyboard::Key::G: chosenColor = sf::Color::Green; break;
                    case sf::Keyboard::Key::B: chosenColor = sf::Color::Blue; break;
                    case sf::Keyboard::Key::Y: chosenColor = sf::Color::Yellow; break;
                    case sf::Keyboard::Key::C: chosenColor = sf::Color::Cyan; break;
                    case sf::Keyboard::Key::M: chosenColor = sf::Color::Magenta; break;
                    default:
                        std::cout << "Invalid color key!\n";
                        return;
                    }
                    sf::Vector2f pos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                    wypewnenia.push_back({ pos, chosenColor });
                    waitingForColor = false;
                    std::cout << "Added fill at (" << pos.x << ", " << pos.y
                        << ") color: " << int(chosenColor.r) << "\n";
                }
                break;
            }
        }
    }
}

/**
 * @brief Aktualizuje wszystkie obiekty.
 * @param dt Delta czasu od ostatniej aktualizacji.
 */
void Engine::update(float dt) {
    for (auto& obj : objects)
        obj->update(dt);
}

/**
 * @brief Renderuje całą scenę: statyczne i animowane obiekty.
 * @param canvas RenderTexture, na którym rysujemy.
 */
void Engine::render(sf::RenderTexture& canvas) {
    PrimitiveRenderer staticRenderer(staticCanvas);

    if (bitmap.getTexture()) {
        sf::Sprite back(*bitmap.getTexture());
        staticCanvas.draw(back);
    }

    // Rysowanie obiektów globalnych
    for (auto& poly : polygony) staticRenderer.drawPolygon(poly, sf::Color::Yellow);
    for (auto& polyline : polyliny) staticRenderer.drawPolyline(polyline, sf::Color::Magenta);
    for (auto& circle : okregy) staticRenderer.drawCircle(circle, 75.0, sf::Color::Green, sf::Color::Green);
    for (auto& ellipse : elipsy) staticRenderer.drawElips(ellipse, 75.0, 100.0, sf::Color::Red, sf::Color::Red);
    for (auto& line : linie) staticRenderer.drawLine(line.first, line.second, sf::Color::Cyan);
    for (auto& fill : wypewnenia) staticRenderer.flood_fill(fill.pos, fill.color, sf::Color::Black);
    for (auto& pt : punkty) staticRenderer.drawPoint(pt, sf::Color::White);

    staticCanvas.display();

    // Render obiektów animowanych
    animatedCanvas.clear(sf::Color::Transparent);
    PrimitiveRenderer animRenderer(animatedCanvas);
    for (auto& obj : objects) {
        if (auto drawable = dynamic_cast<DrawableObject*>(obj.get())) {
            drawable->draw(animRenderer);
        }
    }
    animatedCanvas.display();

    // Wyświetlenie sceny
    window.clear();
    window.draw(sf::Sprite(staticCanvas.getTexture()));
    window.draw(sf::Sprite(animatedCanvas.getTexture()));
    window.display();

    // Czyszczenie buforów rysowania
    linie.clear();
    polyliny.clear();
    polygony.clear();
    okregy.clear();
    elipsy.clear();
    wypewnenia.clear();
}

/**
 * @brief Główna pętla silnika.
 */
void Engine::run() {
    init();
    sf::Clock clock;
    while (isRunning && window.isOpen()) {
        float dt = clock.restart().asSeconds();
        handleInput();
        update(dt);
        render(staticCanvas);
    }
    shutdown();
}

/**
 * @brief Punkt wejścia programu.
 */
int main(void) {
    EngineConfig config;
    config.width = 1280;
    config.height = 720;
    config.fullscreen = true;
    config.fps = 75;
    config.clearColor = sf::Color(0, 0, 0);
    config.windowTitle = "Engine Window!";

    Engine& engine = Engine::getInstance(config);

    // Wczytywanie bitmap gracza (animacje w 4 kierunkach)
    std::vector<std::vector<BitmapHandler>> playerBitmaps(4);
    for (int dir = 0; dir < 4; ++dir) {
        for (int f = 0; f < 4; ++f) {
            BitmapHandler bmp;
            std::string filename = "player_" + std::to_string(dir) + "_" + std::to_string(f) + ".png";
            if (!bmp.loadFromFile(filename)) {
                std::cerr << "Nie udało się załadować bitmapy: " << filename << std::endl;
            }
            playerBitmaps[dir].push_back(bmp);
        }
    }

    auto player = std::make_unique<Player>(playerBitmaps);
    engine.addObject(std::move(player));

    engine.run();
    return 0;
}
