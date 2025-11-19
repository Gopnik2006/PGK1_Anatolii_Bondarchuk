#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <stdexcept>
#include <filesystem>
#include <iostream>

/**
 * @class BitmapHandler
 * @brief Klasa pomocnicza do obsługi bitmap (tekstur) w SFML.
 *
 * Umożliwia wczytywanie, zapisywanie, tworzenie, kopiowanie
 * i czyszczenie obiektów typu sf::Texture. Przechowuje teksturę
 * wewnątrz inteligentnego wskaźnika std::shared_ptr.
 */
class BitmapHandler {
private:
    /**
     * @brief Udostępniana tekstura.
     *
     * Przechowywana jako std::shared_ptr, aby umożliwić współdzielenie
     * tekstur między różnymi obiektami bez kopiowania dużych danych.
     */
    std::shared_ptr<sf::Texture> texture;

public:
    /**
     * @brief Domyślny konstruktor.
     *
     * Tworzy pusty obiekt bez załadowanej tekstury.
     */
    BitmapHandler() = default;

    /**
     * @brief Wczytuje teksturę z pliku.
     *
     * @param filename Ścieżka do pliku graficznego.
     * @return true jeśli udało się wczytać plik, w przeciwnym razie false.
     *
     * Jeśli wystąpi błąd, metoda wypisuje komunikat diagnostyczny
     * wraz z informacją o bieżącym katalogu roboczym.
     */
    bool loadFromFile(const std::string& filename) {
        texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(filename)) {
            std::cout << "ERROR: Cannot load file: " << filename << "\n";
            std::cout << "Current working dir: "
                << std::filesystem::current_path().string() << "\n";

            texture.reset();
            return false;
        }
        return true;
    }

    /**
     * @brief Zapisuje aktualną teksturę do pliku.
     *
     * @param filename Nazwa pliku wynikowego.
     * @return true jeśli zapis się powiódł, false jeśli brak tekstury lub błąd.
     */
    bool saveToFile(const std::string& filename) const {
        if (!texture) return false;
        sf::Image img = texture->copyToImage();
        return img.saveToFile(filename);
    }

    /**
     * @brief Tworzy nową teksturę o podanym rozmiarze i kolorze wypełnienia.
     *
     * @param width  Szerokość tworzonej tekstury.
     * @param height Wysokość tworzonej tekstury.
     * @param color  Kolor, którym ma zostać wypełniona. Domyślnie przezroczysty.
     */
    void create(unsigned int width, unsigned int height, sf::Color color = sf::Color::Transparent) {
        texture = std::make_shared<sf::Texture>();
        sf::Image img({ width, height }, color);
        texture->loadFromImage(img);
    }

    /**
     * @brief Usuwa aktualnie przechowywaną teksturę.
     */
    void clear() { texture.reset(); }

    /**
     * @brief Zwraca wskaźnik do aktualnej tekstury.
     *
     * @return std::shared_ptr<sf::Texture> lub nullptr jeśli brak tekstury.
     */
    std::shared_ptr<sf::Texture> getTexture() const { return texture; }

    /**
     * @brief Kopiuje teksturę z innego obiektu BitmapHandler.
     *
     * @param other Obiekt, z którego tekstura ma zostać skopiowana.
     *
     * Jeśli obiekt `other` nie ma tekstury — operacja jest pomijana.
     */
    void copyFrom(const BitmapHandler& other) {
        if (!other.texture) return;
        texture = std::make_shared<sf::Texture>(*other.texture);
    }
};