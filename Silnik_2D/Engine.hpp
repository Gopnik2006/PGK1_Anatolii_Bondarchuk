#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <iostream>

enum class WindowMode {
    WINDOWED,
    FULLSCREEN
};

class Engine {
public:
    Engine();
    ~Engine();

    bool init(const std::string& title, int width, int height,
        WindowMode mode = WindowMode::WINDOWED,
        int fps = 60, bool useMouse = true, bool useKeyboard = true);

    void run();                       // główna pętla gry
    void clearScreen(const sf::Color& color);  // czyszczenie ekranu
    void handleEvents();               // obsługa wejścia
    void logError(const std::string& message); // logowanie błędów

private:
    sf::RenderWindow* window;
    sf::Clock clock;
    int targetFPS;
    bool running;
    bool mouseEnabled;
    bool keyboardEnabled;
    std::ofstream logFile;

    void update(float deltaTime);  // aktualizacja logiki gry
    void render();                 // rysowanie
    void shutdown();               // zamknięcie silnika
};

#endif // ENGINE_HPP