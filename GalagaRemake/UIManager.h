#pragma once
#include <vector>
#include "Ship.h"
#include "TempText.h"
#include "PlayerShip.h"

class UIManager
	: public sf::Drawable
{
public:
	UIManager(const PlayerShip& playerShip, const sf::Font& font, const BoundedFloatRect& windowDimensions,
		int totalExtraLives = 3, float windowMargin = 10.f);
	void addUiText(TempText text);
	void gameOver();
	bool isOutOfLives();
	void playerLostLife();
	void updateUI(int currentScore);
	void initializeLevelIntroText(TempText& primaryText, TempText& secondaryText);
	void initializeLevelOutroText(TempText& primaryText, TempText& secondaryText);

private:
	void updateTempText();
	void updateHealthBar();
	sf::Vector2f centerElement(sf::FloatRect rect) const;
	void initializeLives(const Ship& livesShipRepresentation, int totalExtraLives);
	void initializeScore();
	void initializeGameOverText();
	void initializeExtraLivesText();
	void initializeHealthSegments();
	void initializeHealthBarSegment();
	void initializePlayerHealth(const PlayerShip& playerShip);
	void reinitializePlayerHealth();

	std::vector<Ship> m_lives;
	std::vector<sf::RectangleShape> m_healthBarSegments;
	std::vector<TempText> m_texts;
	bool m_isGameOver = false;
	bool m_playerDied = false;
	sf::Text m_gameOverText;
	sf::Text m_scoreText;
	sf::Text m_extraLivesText;
	sf::Vector2f m_baseScale;
	sf::RectangleShape m_healthBarSegment;
	const float m_windowMargin;
	const int& m_playerHealth;
	BoundedFloatRect m_windowDimensions;
	const sf::Font& m_font;



	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

