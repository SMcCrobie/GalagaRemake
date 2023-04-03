#include "BackgroundManager.h"
#include "RandMacros.h"

void BackgroundManager::init()
{
	m_windowBuffer = 100.f;
	m_planetMovementCounter = 0;
	m_planetColorOffest = 166;

	for (int i = 0; i < 50; i++) {
		m_stars.push_back(sf::CircleShape(2.5f));
		sf::CircleShape& star = m_stars.back();
		star.setFillColor(i == 0 ? sf::Color(102, 217, 255, 200) : sf::Color(255, 255, 255, 200));
		star.setOutlineColor(i == 0 ? sf::Color(153, 230, 255, 80) : sf::Color(255, 255, 230, 80));
		star.setOutlineThickness(2.5f);

		const float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(m_windowDimensions.left, m_windowDimensions.right);
		const float yCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(m_windowDimensions.top - m_windowBuffer, m_windowDimensions.bottom + m_windowBuffer);
		star.setPosition(xCoordinate, yCoordinate);
	}
}

BackgroundManager::BackgroundManager(BoundedFloatRect windowDimensions)
	: m_windowDimensions(windowDimensions)
{
	init();
}

void BackgroundManager::moveBackground(float increment)
{
	for (auto& star : m_stars) {
		star.move(0, increment);
		if (star.getGlobalBounds().top > m_windowDimensions.bottom + m_windowBuffer) {
			const float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(m_windowDimensions.left, m_windowDimensions.right);
			star.setPosition(xCoordinate, m_windowDimensions.top - m_windowBuffer);
		}
	}
	m_foregroundPlanet.move(-increment/8, increment/8);
	m_foregroundPlanet.setScale(m_foregroundPlanet.getScale() * 1.0002f);
	
}

void BackgroundManager::addForegroundPlanet(const sf::Sprite planet)
{
	m_foregroundPlanet = planet;
	m_forgroundPlanetStartScale = planet.getScale();
	m_foregroundPlanetStartPosition = planet.getPosition();
}

void BackgroundManager::resetManager()
{
	m_foregroundPlanet.setPosition(m_foregroundPlanetStartPosition);
	m_foregroundPlanet.setScale(m_forgroundPlanetStartScale);
	m_stars.clear();
	init();
}

void BackgroundManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& star : m_stars) {
		//if(!star.getGlobalBounds().intersects(m_foregroundPlanet.getGlobalBounds()))
		target.draw(star);
	}
	target.draw(m_foregroundPlanet);
}
