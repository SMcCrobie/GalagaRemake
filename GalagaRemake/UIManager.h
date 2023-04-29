#pragma once
#include <vector>
#include "Ship.h"
#include "TempText.h"
#include "PlayerShip.h"
#include "Fonts.h"

class UIManager
	: public sf::Drawable, public Manager
{
public:
	UIManager(const PlayerShip& playerShip, const BoundedFloatRect& windowDimensions,
	          int totalExtraLives = 3, float windowMargin = 10.f);
	void addUiText(TempText text);
	bool isOutOfLives() const;
	void playerLostLife();
	void updateUI(int currentScore);
	void stylePrimaryText(sf::Text& primaryText);
	void styleSecondaryText(sf::Text& secondaryText);
	void initializeLevelIntroText(TempText& primaryText, TempText& secondaryText);
	void initializeLevelOutroText(TempText& primaryText, TempText& secondaryText);
	void resetManager() override;

private:
	void initializePauseText();
	void updateTempText();
	void updateHealthBar();
	sf::Vector2f centerElement(sf::FloatRect rect) const;
	void initializeLives();
	void initializeScore();
	void initializeGameOverText();
	void initializeExtraLivesText();
	void initializeHealthSegments();
	void initializeHealthBarSegment();
	void initializePlayerHealth();
	

	std::vector<Ship> m_lives;
	std::vector<sf::RectangleShape> m_healthBarSegments;
	std::vector<TempText> m_texts;
	sf::Text m_gameOverText;
	sf::Text m_pauseText;
	sf::Text m_pauseTextByline;
	sf::Text m_pauseTextByline2;
	sf::Text m_scoreText;
	sf::Text m_extraLivesText;
	sf::Vector2f m_baseScale;
	sf::RectangleShape m_healthBarSegment;
	const float m_windowMargin;
	const int& m_playerHealth;
	const Ship& m_lifeSymbol;
	int m_totalExtraLives;
	BoundedFloatRect m_windowDimensions;
	TempText m_introPrimaryText;
	TempText m_introSecondaryText;
	sf::Text m_gameOverByline;
	sf::Text m_gameOverByline2;


	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

