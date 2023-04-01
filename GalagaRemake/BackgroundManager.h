#pragma once
#include <SFML/Graphics.hpp>
#include "BoundedFloatRect.h"

class BackgroundManager
	: public sf::Drawable
{
public:
	BackgroundManager(BoundedFloatRect windowDimensions);
	void moveBackground(float increment);
	void addForegroundPlanet(sf::Sprite planet);

private:
	std::vector <sf::CircleShape> m_stars;
	sf::Sprite m_foregroundPlanet;
	BoundedFloatRect m_windowDimensions;
	float m_windowBuffer;
	int m_planetMovementCounter;
	int m_planetColorOffest;

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

