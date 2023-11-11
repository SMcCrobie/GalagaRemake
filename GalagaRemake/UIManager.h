#pragma once
#include <vector>
#include "Ship.h"
#include "TempText.h"
#include "PlayerShip.h"
#include "Fonts.h"

class UIManager
	: public sf::Drawable, public IManager
{
public:
	UIManager();
	void init(const Ship& shipModel, int totalExtraLives = 3, float windowMargin = 10.f);
	void addUiText(TempText text);
	bool isOutOfLives() const;
	void playerLostLife();
	void updateUI();
	void stylePrimaryText(sf::Text& primaryText);
	void styleSecondaryText(sf::Text& secondaryText);
	void initializeLevelIntroText(TempText& primaryText, TempText& secondaryText);
	void initializeLevelOutroText(TempText& primaryText, TempText& secondaryText);
	void resetManager() override;
	void addPointValue(sf::Vector2f position, int pointValue, sf::Color color = sf::Color::White, float scale = 0.f);


private:
	void initializePauseText();
	void updateTempText();
	void updateHealthBar();
	sf::Vector2f centerElement(sf::FloatRect rect) const;

	void initializeLives();
	void initializeScore();
	void initializeGameOverText();
	void initializeExtraLivesText();
	void initializeHealthBar();
	void initializeHealthBarSegment();
	bool isHealthBarSegmentInitialized();
	void initializePlayerHealth();
	void initializeShipPointValue();
	

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
	float m_windowMargin;
	Ship m_lifeSymbol;
	int m_totalExtraLives;
	BoundedFloatRect m_windowDimensions;
	TempText m_introPrimaryText;
	TempText m_introSecondaryText;
	TempText m_shipPointValue;
	sf::Text m_gameOverByline;
	sf::Text m_gameOverByline2;


	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

