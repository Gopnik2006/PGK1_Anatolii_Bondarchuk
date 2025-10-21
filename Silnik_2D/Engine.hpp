#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

struct EngineConfig {
    unsigned int width = 800;
    unsigned int height = 600;
    bool fullscreen = false;
    unsigned int fps = 60;
    sf::Color clearColor = sf::Color::Black;
    std::string windowTitle = "Engine Window";
};

class Engine {
private:
    static Engine* instance;
    sf::RenderWindow window;
    bool isRunning = false;
    sf::Color clearColor;

    // Singleton
    Engine(const EngineConfig& config);

public:
    // Dostęp do Singletona
    static Engine& getInstance(const EngineConfig& config) {
        if (!instance) {
            instance = new Engine(config);
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
    void update(float dt);      
    void render();
    void shutdown();
};
