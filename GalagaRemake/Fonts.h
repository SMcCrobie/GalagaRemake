#pragma once
#include <SFML/Graphics/Font.hpp>
//font definitions
#define PRIMARY_COLOR sf::Color::White
#define SECONDARY_COLOR sf::Color(0x05ecf1ff)

namespace Fonts
{
	//fonts
	extern sf::Font galaxus;
	extern sf::Font play_fair;
	extern sf::Font roboto;

	//Load all fonts from files
	void load();
}
