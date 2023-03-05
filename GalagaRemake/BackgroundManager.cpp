#include "BackgroundManager.h"

BackgroundManager::BackgroundManager(BoundedFloatRect windowDimensions)
	: m_stars(), m_windowDimensions(windowDimensions), m_windowBuffer(100.f)
	, m_planetMovementCounter(0), m_planetColorOffest(166)
{
	for (int i = 0; i < 50; i++) {
		m_stars.push_back(sf::CircleShape(2.5f));
		sf::CircleShape& star = m_stars.back();
		star.setFillColor(i == 0 ? sf::Color(102, 217, 255, 150) : sf::Color(155, 255, 255, 150));
		star.setOutlineColor(i == 0 ? sf::Color(153, 230, 255, 80) : sf::Color(255, 255, 230, 80));
		star.setOutlineThickness(2.5f);

		float xCoordinate = m_windowDimensions.left + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (m_windowDimensions.right - m_windowDimensions.left)));
		float yCoordinate = (m_windowDimensions.top - m_windowBuffer) + static_cast <float> (rand()) / 
			(static_cast <float> (RAND_MAX / ((m_windowDimensions.bottom + m_windowBuffer) - (m_windowDimensions.top - m_windowBuffer))));
	
		star.setPosition(xCoordinate, yCoordinate);
	}
}

void BackgroundManager::moveBackground(float increment)
{
	for (auto& star : m_stars) {
		star.move(0, increment);
		if (star.getGlobalBounds().top > m_windowDimensions.bottom + m_windowBuffer) {
			float xCoordinate = m_windowDimensions.left + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (m_windowDimensions.right - m_windowDimensions.left)));
			star.setPosition(xCoordinate, m_windowDimensions.top - m_windowBuffer);
		}
	}
	if (m_planetMovementCounter % 8 == 0) {
		m_foregroundPlanet.move(0, increment);
		m_foregroundPlanet.setScale(m_foregroundPlanet.getScale() * 1.0001f);
		m_foregroundPlanet.setColor(sf::Color(m_planetColorOffest, m_planetColorOffest, m_planetColorOffest));
		if (m_planetMovementCounter % 128 == 0 && m_planetColorOffest < 255) {
			m_planetColorOffest++;
		}
	}
	m_planetMovementCounter++;
}

void BackgroundManager::addForegroundPlanet(sf::Sprite planet)
{
	m_foregroundPlanet = planet;
}

void BackgroundManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& star : m_stars) {
		target.draw(star);
	}
	target.draw(m_foregroundPlanet);
}
