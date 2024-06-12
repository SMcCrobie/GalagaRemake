#pragma once
#include <SFML/Graphics.hpp>
#include "BoundedFloatRect.h"
#include "IManager.h"

class BackgroundManager
	: public sf::Drawable, public IManager
{
public:
	BackgroundManager();
	void moveBackground(float increment);
	void addForegroundPlanet(sf::Sprite planet);
	void removeForegroundPlanet();
	void resetManager() override;

private:
	void init();
	std::vector <sf::CircleShape> m_stars;
	BoundedFloatRect m_windowDimensions;
	float m_windowBuffer;
	int m_planetMovementCounter;
	int m_planetColorOffest;

	sf::Sprite m_foregroundPlanet;
	sf::Vector2f m_forgroundPlanetStartScale;
	sf::Vector2f m_foregroundPlanetStartPosition;

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

