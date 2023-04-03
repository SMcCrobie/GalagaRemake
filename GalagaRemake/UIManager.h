#pragma once
#include <vector>
#include "Ship.h"
#include "TempText.h"
#include "PlayerShip.h"

class UIManager
	: public sf::Drawable, public Manager
{
public:
	UIManager(const PlayerShip& playerShip, const sf::Font& font, const BoundedFloatRect& windowDimensions,
		int totalExtraLives = 3, float windowMargin = 10.f);
	void addUiText(TempText text);
	bool isOutOfLives() const;
	void playerLostLife();
	void updateUI(int currentScore);
	void initializeLevelIntroText(TempText& primaryText, TempText& secondaryText);
	void initializeLevelOutroText(TempText& primaryText, TempText& secondaryText);
	void resetManager() override;

private:
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
	sf::Text m_scoreText;
	sf::Text m_extraLivesText;
	sf::Vector2f m_baseScale;
	sf::RectangleShape m_healthBarSegment;
	const float m_windowMargin;
	const int& m_playerHealth;
	const Ship& m_lifeSymbol;
	int m_totalExtraLives;
	BoundedFloatRect m_windowDimensions;
	const sf::Font& m_font;
	TempText m_introPrimaryText;
	TempText m_introSecondaryText;



	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

