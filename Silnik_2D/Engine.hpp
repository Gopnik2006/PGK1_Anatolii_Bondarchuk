#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"

/**
 * @struct EngineConfig
 * @brief Konfiguracja silnika graficznego i okna aplikacji.
 *
 * Zawiera ustawienia takie jak rozdzielczość, tryb pełnoekranowy,
 * limit FPS, kolor czyszczenia oraz tytuł okna.
 */
struct EngineConfig {
    unsigned int width = 800;               ///< Szerokość okna.
    unsigned int height = 600;              ///< Wysokość okna.
    bool fullscreen = false;                ///< Czy uruchomić w trybie pełnoekranowym.
    unsigned int fps = 60;                  ///< Docelowa liczba klatek na sekundę.
    sf::Color clearColor = sf::Color::Black;///< Kolor używany do czyszczenia ekranu.
    std::string windowTitle = "Engine Window"; ///< Tytuł okna.
};

/**
 * @class Engine
 * @brief Główny silnik aplikacji (Singleton).
 *
 * Odpowiada za:
 * - tworzenie i obsługę okna SFML,
 * - zarządzanie pętlą gry,
 * - obsługę wejścia,
 * - aktualizację logiki obiektów,
 * - renderowanie na dwóch warstwach: statycznej i animowanej,
 * - obsługę bitmap przez BitmapHandler,
 * - przechowywanie obiektów implementujących UpdatableObject.
 *
 * Silnik działa jako Singleton — dostęp przez Engine::getInstance().
 */
class Engine {
private:
    static Engine* instance;               ///< Wskaźnik do jedynej instancji Singletona.

    sf::RenderWindow window;               ///< Główne okno aplikacji SFML.
    bool isRunning = false;                ///< Flaga działania głównej pętli gry.
    sf::Color clearColor;                  ///< Kolor czyszczenia sceny.

    sf::RenderTexture staticCanvas;        ///< Warstwa statyczna (tło, prymitywy, bitmapy).
    sf::RenderTexture animatedCanvas;      ///< Warstwa animowana (gracz, obiekty ruchome).
    sf::Sprite canvasSprite;               ///< Sprite łączący warstwy do finalnego renderingu.

    /**
     * @brief Prywatny konstruktor — część wzorca Singleton.
     *
     * @param config Konfiguracja silnika.
     */
    Engine(const EngineConfig& config);

    std::vector<std::unique_ptr<UpdatableObject>> objects; ///< Lista obiektów podlegających aktualizacji.

public:
    BitmapHandler bitmap; ///< Obsługa bitmap — wczytywanie, zapisywanie, generowanie.

    /**
     * @brief Pobiera instancję silnika (Singleton).
     *
     * Jeśli instancja nie istnieje — tworzy ją z podaną konfiguracją.
     *
     * @param config Konfiguracja silnika (używana tylko przy pierwszym wywołaniu).
     * @return Referencja do jedynej instancji Engine.
     */
    static Engine& getInstance(const EngineConfig& config) {
        if (!instance) {
            instance = new Engine(config);
        }
        return *instance;
    }

    // --- Blokada kopiowania (Singleton) ---
    Engine(const Engine&) = delete;               ///< Konstruktor kopiujący zablokowany.
    Engine& operator=(const Engine&) = delete;    ///< Operator przypisania zablokowany.

    // --- Główne funkcje silnika ---

    /**
     * @brief Ładuje bitmapę z pliku i umieszcza ją na warstwie statycznej.
     *
     * @param filename Ścieżka do pliku.
     * @return true jeśli udało się wczytać bitmapę, false w przeciwnym razie.
     */
    bool loadBitmapToCanvas(const std::string& filename);

    /**
     * @brief Zapisuje aktualny stan warstwy statycznej do pliku.
     *
     * @param filename Nazwa pliku wynikowego.
     * @return true jeśli zapis zakończył się sukcesem.
     */
    bool saveCanvasToFile(const std::string& filename);

    /**
     * @brief Tworzy pustą warstwę statyczną o podanym rozmiarze i kolorze.
     *
     * @param w Szerokość.
     * @param h Wysokość.
     * @param c Kolor wypełnienia.
     */
    void createBlankCanvas(unsigned w, unsigned h, sf::Color c);

    /**
     * @brief Inicjalizuje silnik: tworzy okno, ustawia canvas i parametry renderowania.
     */
    void init();

    /**
     * @brief Obsługuje wejście użytkownika (klawiatura, mysz).
     */
    void handleInput();

    /**
     * @brief Uruchamia główną pętlę gry.
     *
     * Odpowiada za:
     * - dt (delta-time),
     * - update(),
     * - render(),
     * - synchronizację FPS.
     */
    void run();

    /**
     * @brief Aktualizuje wszystkie obiekty dodane do silnika.
     *
     * @param dt Czas w sekundach od poprzedniej klatki.
     */
    void update(float dt);

    /**
     * @brief Renderuje obecną scenę do podanego canvasu.
     *
     * @param canvas RenderTexture, do którego mają być rysowane warstwy.
     */
    void render(sf::RenderTexture& canvas);

    /**
     * @brief Zamyka silnik — kończy działanie pętli gry.
     */
    void shutdown();

    /**
     * @brief Dodaje obiekt do aktualizacji w pętli gry.
     *
     * @param obj Obiekt implementujący UpdatableObject.
     */
    void addObject(std::unique_ptr<UpdatableObject> obj) {
        objects.push_back(std::move(obj));
    }
};