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

Engine* Engine::instance = nullptr;

Engine::Engine(const EngineConfig& config) {
    sf::VideoMode mode;
    mode.size = { config.width, config.height }; 
    clearColor = config.clearColor;
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
        // Obsługa klawiatury, myszy.
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                isRunning = false;
                window.close();
            }
            if (keyPressed->code == sf::Keyboard::Key::Enter) {
                
            }
            if (keyPressed->code == sf::Keyboard::Key::Space) {
                
            }
        }
        if (const auto* MousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (MousePressed->button == sf::Mouse::Button(0)) {
                
            }
        }
    }
}

// Aktualizacja logiki gry
void Engine::update(float dt) {
    // Tutaj dodajesz logikę gry np. ruchy gracza, AI, kolizje
}

// Rysowanie obiektów gry
void Engine::render() {
    window.clear(clearColor); 
    PrimitiveRenderer renderer(&window);
    window.display(); 
}

void Engine::run() {
    init();
    sf::Clock clock;
    render();
    // Główna pętla
    while (isRunning && window.isOpen()) {
        float dt = clock.restart().asSeconds();
        handleInput();
        window.clear(clearColor);
        window.display();
    }
    shutdown();
}

int main(void) {
    
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