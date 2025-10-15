#include "Engine.hpp"
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/ContextSettings.hpp>
Engine::Engine()
    : window(nullptr),
    targetFPS(60),
    running(false),
    mouseEnabled(true),
    keyboardEnabled(true)
{
    logFile.open("engine_log.txt", std::ios::out);
    if (!logFile.is_open()) {
        std::cerr << "Nie udało się otworzyć pliku logów!" << std::endl;
    }
}

Engine::~Engine() {
    shutdown();
}

bool Engine::init(const std::string& title, int width, int height,
    WindowMode mode, int fps, bool useMouse, bool useKeyboard)
{
    targetFPS = fps;
    mouseEnabled = useMouse;
    keyboardEnabled = useKeyboard;

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    sf::VideoMode videoMode({width,height},desktop);
    const sf::ContextSettings Default;
    sf::State state ;

    window = new sf::RenderWindow(videoMode, title, state, Default);

    if (!window->isOpen()) {
        logError("Nie udało się utworzyć okna SFML.");
        return false;
    }

    window->setFramerateLimit(targetFPS);
    running = true;
    return true;
}

void Engine::run() {
    if (!window) {
        logError("Silnik nie został zainicjalizowany przed wywołaniem run().");
        return;
    }

    while (running && window->isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        handleEvents();
        update(deltaTime);
        render();
    }

    shutdown();
}

void Engine::clearScreen(const sf::Color& color) {
    if (window)
        window->clear(color);
}

void Engine::handleEvents() {
    
}

void Engine::update(float deltaTime) {
    // Tutaj w przyszłości logika gry
}

void Engine::render() {
    clearScreen(sf::Color(30, 30, 30)); // np. ciemnoszary
    // Tutaj rysowanie obiektów
    window->display();
}

void Engine::logError(const std::string& message) {
    if (logFile.is_open())
        logFile << "[ERROR] " << message << std::endl;
    else
        std::cerr << "[ERROR] " << message << std::endl;
}

void Engine::shutdown() {
    if (window) {
        window->close();
        delete window;
        window = nullptr;
    }

    if (logFile.is_open()) {
        logFile << "Silnik zakończył pracę." << std::endl;
        logFile.close();
    }

    running = false;
}
