#include "Engine.hpp"
#include "LineSegment.hpp"
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <stdio.h>
#include <variant>
#include <optional>
#include <fstream>
#include <cstdint>
#include <Windows.h>

Engine* Engine::instance = nullptr;

std::vector<sf::Vector2f> punkty;
std::vector<std::pair<sf::Vector2f, sf::Vector2f>> linie;
std::vector<std::vector<sf::Vector2f>> polyliny;
std::vector<std::vector<sf::Vector2f>> polygony;

Engine::Engine(const EngineConfig& config) {
    sf::VideoMode mode;
    mode.size = { config.width, config.height }; 
    clearColor = config.clearColor;
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    window = sf::RenderWindow(mode, config.windowTitle); 
    window.setFramerateLimit(config.fps);
}

static void log(const std::string& message) {
    std::ofstream logFile("engine.log", std::ios::app);
    logFile << "[LOG] " << message << "\n";
}

void Engine::shutdown() {
    log("Shutting down engine...");
    window.close();
    ::ShowWindow(::GetConsoleWindow(), SW_SHOW);

    // sprzątanie pamięci
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

void Engine::init() {
    isRunning = true;
    std::cout << "[Engine] Initialized\n";
}

// Obsługa wszystkich zdarzeń
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
                punkty.push_back({ pos });
            }
        }
        // Obsługa klawiatury
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                isRunning = false;
                window.close();
            }
            if (keyPressed->code == sf::Keyboard::Key::Space) {
                sf::Vector2f pos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                linie.push_back({ pos, pos + sf::Vector2f(50, 50) });
            }
            if (keyPressed->code == sf::Keyboard::Key::U){
                punkty.clear();
                linie.clear();
                polygony.clear();
            }
            if (keyPressed->code == sf::Keyboard::Key::P) {
                polygony.push_back( punkty );
                punkty.clear();
            }
            if (keyPressed->code == sf::Keyboard::Key::L) {
                polyliny.push_back(punkty);
                punkty.clear();
            }
        }
    }
}

// Aktualizacja logiki gry
void Engine::update(float dt) {
    
}

// Rysowanie obiektów gry
void Engine::render() {
    window.clear(clearColor); 
    PrimitiveRenderer renderer(&window);

    for (auto& polygon : polygony)
        renderer.drawPolygon(polygon, sf::Color::Yellow);
    for (auto& polyline : polyliny)
        renderer.drawPolyline(polyline, sf::Color::Magenta);
    for (auto& punkt : punkty)
        renderer.drawPoint(punkt, sf::Color::White);
    for (auto& linia : linie)
        renderer.drawLine(linia.first, linia.second, sf::Color::Cyan);

    window.display(); 
}

void Engine::run() {
    init();
    sf::Clock clock;
    // Główna pętla
    while (isRunning && window.isOpen()) {
        float dt = clock.restart().asSeconds();
        handleInput();
        update(dt);
        render();
    }
    shutdown();
}

int main(void) {
    // Konfiguracja
    EngineConfig config;
    config.width = 1280;
    config.height = 720;
    config.fullscreen = false;
    config.fps = 75;
    config.clearColor = sf::Color(60, 60, 60);
    config.windowTitle = "Engine Window!";

    Engine& engine = Engine::getInstance(config);

    engine.run();
    return 0;
}