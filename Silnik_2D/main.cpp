
#include "Engine.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480, 32), "Bug");

    // Some bug specific code here...

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // ... and here.

        window.display();
    }

    return EXIT_SUCCESS;
}