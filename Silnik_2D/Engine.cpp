#include "Engine.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <stdio.h>
#include <variant>
#include <optional>


Engine* Engine::instance = nullptr;

Engine::Engine() {
    // ✅ SFML 3.0.2 - poprawna składnia tworzenia okna
    sf::VideoMode mode;
    mode.size = { 800, 600 }; // 👈 Nowa składnia, nie VideoMode({800,600})
    window = sf::RenderWindow(mode, "Engine - SFML 3.0.2"); // ✅ poprawne
}

void Engine::init() {
    isRunning = true;
    window.setFramerateLimit(60);
    std::cout << "[Engine] Initialized\n";
}

void Engine::handleInput() {
    
    
}

void Engine::run() {
    init();
    sf::Clock clock;

    while (isRunning && window.isOpen()) {
        float dt = clock.restart().asSeconds();
        handleInput();
        window.clear();
        window.display();
    }
}
