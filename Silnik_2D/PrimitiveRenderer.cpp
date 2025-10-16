#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <optional>
#include <string_view>
#include <vector>


class PrimitiveRenderer {
public:
	sf::Vector2u pos;
	sf::Vector2u size = {800,600};
	PrimitiveRenderer(sf::Vector2u pos) {
		this->pos = pos;
	};

	
	void RysujPixel() {
		sf::Image image(size, sf::Color::Black);
		image.setPixel(pos, sf::Color::White);
	}

};