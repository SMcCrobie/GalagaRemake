#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "Manager.h"

class Level
	: public sf::Drawable
{
public:
	Level& addManager(const std::shared_ptr<Manager>& manager);
	Level& addDrawable(const std::shared_ptr<sf::Drawable>& drawable);
	bool checkForGameEvent() const;
	void reset() const;

private:
	std::vector<std::shared_ptr<sf::Drawable>> m_drawables;
	std::vector<std::shared_ptr<Manager>> m_managers;

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

