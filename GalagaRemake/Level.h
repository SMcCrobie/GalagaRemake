#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "Controller.h"
#include "Manager.h"

class Level
	: public sf::Drawable
{
public:
	Level& addManager(const std::shared_ptr<Manager>& manager);
	Level& addDrawableLayer(const std::shared_ptr<sf::Drawable>& drawable);
	bool checkForGameEvent(KeyboardController& playerController) const;
	void reset(KeyboardController& playerController) const;

private:
	std::vector<std::shared_ptr<sf::Drawable>> m_drawables;
	std::vector<std::shared_ptr<Manager>> m_managers;

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

