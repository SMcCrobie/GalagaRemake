#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "GameState.h"

namespace Window
{
	inline const unsigned int screen_height = sf::VideoMode::getDesktopMode().height / 10 * 7;//70% of screen height


	inline void create_window_dynamically(sf::RenderWindow& window)
	{
		window.create(sf::VideoMode(screen_height / 5 * 3, screen_height), "Galaga!");
		sf::View view(sf::FloatRect(0.f, 0.f, GameState::world_bounds.width, GameState::world_bounds.height));
		view.setViewport(sf::FloatRect(0, 0, 1, 1)); // full viewport
		window.setView(view);
		window.setKeyRepeatEnabled(false);
	}

	inline void reset_window_size_dynamically(sf::RenderWindow& window){
		window.setSize(sf::Vector2u(screen_height / 5 * 3, screen_height));
		GameState::resetWindow = false;
		GameState::ignoreNextResizeEvent = true;
	}

}