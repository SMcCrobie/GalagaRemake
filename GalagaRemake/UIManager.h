#pragma once
#include <vector>
#include "Ship.h"
#include <iostream>

class UIManager
	: public sf::Drawable
{
public:
	UIManager(const Ship& livesShipRepresentation, const sf::Font& font, BoundedFloatRect windowDimensions, 
		int totalExtraLives = 3, float windowMargin = 10.f);
	sf::Text& addUiText(std::string text);
	void gameOver();
	bool isOutOfLives();
	void playerLostLife();
	void updateUI(int currentScore);

private:
	void initializeLives(const Ship& livesShipRepresentation, int totalExtraLives);
	void initializeScore();
	void initializeGameOverText();
	void initializeExtraLivesText();

	std::vector<Ship> m_lives;
	std::vector<sf::Text> m_texts; //maybe in the future
	bool m_isGameOver = false;
	bool m_playerDied = false;
	sf::Text m_gameOverText;
	sf::Text m_scoreText;
	sf::Text m_extraLivesText;
	sf::Vector2f m_baseScale;
	const float m_windowMargin;
	BoundedFloatRect m_windowDimensions;
	const sf::Font& m_font;



	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

