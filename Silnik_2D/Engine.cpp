#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>

// Przykładowa klasa bazowa dla obiektów gry
class GameObject {
public:
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual ~GameObject() = default;
};

class Engine {
private:
    // Singleton Instance
    static Engine* instance;

    // Okno gry
    sf::RenderWindow window;
    sf::Event event;

    // Kontener na obiekty gry
    std::vector<std::shared_ptr<GameObject>> gameObjects;

    // Czy gra działa?
    bool isRunning = false;

    // Prywatny konstruktor (Singleton)
    Engine() : window(sf::VideoMode(800, 600), "Game Engine - SFML") {}

public:
    // Usuwamy kopiowanie i przenoszenie (dla bezpieczeństwa Singletona)
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    // 🔹 Pobranie instancji Singletona
    static Engine& getInstance() {
        if (!instance) {
            instance = new Engine();
        }
        return *instance;
    }

    // 🔹 Inicjalizacja silnika
    void init() {
        isRunning = true;
        window.setFramerateLimit(60);
        std::cout << "[Engine] Inicjalizacja silnika zakończona.\n";
    }

    // 🔹 Dodawanie obiektów gry
    void addGameObject(const std::shared_ptr<GameObject>& obj) {
        gameObjects.push_back(obj);
    }

    // 🔹 Obsługa wejścia (klawiatura, mysz)
    void handleInput() {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                isRunning = false;
        }
    }

    // 🔹 Aktualizacja logiki gry
    void update(float dt) {
        for (auto& obj : gameObjects) {
            obj->update(dt);
        }
    }

    // 🔹 Renderowanie obiektów gry
    void render() {
        window.clear();
        for (auto& obj : gameObjects) {
            obj->render(window);
        }
        window.display();
    }

    // 🔹 Główna pętla gry
    void run() {
        init();
        sf::Clock clock;

        while (isRunning && window.isOpen()) {
            float dt = clock.restart().asSeconds();
            handleInput();
            update(dt);
            render();
        }

        shutdown();
    }

    // 🔹 Zamykanie gry i sprzątanie
    void shutdown() {
        std::cout << "[Engine] Czyszczenie zasobów i zamykanie silnika.\n";
        window.close();
    }
};

// Inicjalizacja statycznego wskaźnika
Engine* Engine::instance = nullptr;
