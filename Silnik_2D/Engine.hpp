#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

class Engine {
private:
    static Engine* instance;
    sf::RenderWindow window;
    bool isRunning = false;

    // Prywatny konstruktor = Singleton
    Engine();

public:
    // Dostęp do instancji Singletona
    static Engine& getInstance() {
        if (!instance) {
            instance = new Engine();
        }
        return *instance;
    }

    // Blokada kopiowania (Singleton)
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    // Główne funkcje
    void init();
    void handleInput();
    void run();
};
