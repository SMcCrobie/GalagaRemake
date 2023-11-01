#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "GameState.h"

namespace Window
{
	void create_window_dynamically(sf::RenderWindow& window)
	{
		unsigned int screenHeight = sf::VideoMode::getDesktopMode().height / 10 * 7;//70% of screen height
		window.create(sf::VideoMode(screenHeight / 5 * 3, screenHeight), "Galaga!");
		sf::View view(sf::FloatRect(0.f, 0.f, GameState::world_bounds.width, GameState::world_bounds.height));
		view.setViewport(sf::FloatRect(0, 0, 1, 1)); // full viewport
		window.setView(view);
		window.setKeyRepeatEnabled(false);
	}

}