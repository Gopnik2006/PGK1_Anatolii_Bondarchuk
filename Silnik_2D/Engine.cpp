#include "Engine.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <stdio.h>
#include <variant>
#include <optional>
#include "PrimitiveRenderer.cpp"

Engine* Engine::instance = nullptr;

Engine::Engine() {
    sf::VideoMode mode;
    mode.size = { 800, 600 }; 
    window = sf::RenderWindow(mode, "Engine - SFML 3.0.2"); 
}

void Engine::init() {
    isRunning = true;
    window.setFramerateLimit(60);
    std::cout << "[Engine] Initialized\n";
}

// Obsługa wszystkich zdarzeń
void Engine::handleInput() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            isRunning = false;
            window.close();
        }
        // Tutaj możesz dodać obsługę klawiatury, myszy, itp.
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
                sf::Vector2i pi = sf::Mouse::getPosition();
                sf::Vector2u pu(static_cast<unsigned int>(pi.x), static_cast<unsigned int>(pi.y));
                PrimitiveRenderer* pr = new PrimitiveRenderer(pu);
                pr->RysujPixel();
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
    window.clear(sf::Color::Black); // Czyszczenie ekranu
    // Tutaj rysujesz wszystkie obiekty np. window.draw(sprite);
    window.display(); // Wyświetlenie narysowanej ramki
}

void Engine::run() {
    init();
    sf::Clock clock;
    // Główna pętla
    while (isRunning && window.isOpen()) {
        float dt = clock.restart().asSeconds();
        handleInput();
        window.clear();
        window.display();
    }
}

int main(void) {
    Engine& engine = Engine::getInstance();

    // Uruchomienie pętli silnika
    engine.run();
    return 0;
}