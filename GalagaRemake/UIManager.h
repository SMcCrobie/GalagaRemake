#pragma once
#include <vector>
#include "Ship.h"
#include "TempText.h"
#include "PlayerShip.h"
#include "Fonts.h"
#include "MultiColorText.h"

#define CENTER_ELEMENT_HORIZONTALLY(elementWidth) ((GameState::world_bounds.width - (elementWidth)) / 2)
#define CENTER_ELEMENT_VERTICALLY(elementHeight) ((GameState::world_bounds.height - (elementHeight)) / 2)



class UIManager
	: public sf::Drawable, public IManager
{
public:
	void init(const Ship& shipModel, int totalExtraLives = 3, float windowMargin = 10.f);
	void addUiText(TempText text);
	bool isOutOfLives() const;
	void playerLostLife();
	void updateUI();
	void initMainMenuText();
	static void formatPrimaryText(sf::Text& primaryText);
	static void formatSecondaryText(sf::Text& secondaryText, sf::FloatRect primaryText);
	void initializeLevelIntroText(TempText& primaryText, TempText& secondaryText);
	void initializeLevelOutroText(TempText& primaryText, TempText& secondaryText);
	void resetManager() override;
	void addPointValue(sf::Vector2f position, int pointValue, sf::Color color = sf::Color::White, float scale = 0.f);
	void hideUI();
	void unHideUI();


private:
	template<typename TextType>
	static void formatSecondaryText(MultiColorText<TextType>& secondaryText, const sf::FloatRect& primaryText);
	void initializePauseText();
	void updateTempText();
	void updateHealthBar();
	static sf::Vector2f getPrimaryTitlePosition(sf::FloatRect rect);
	static sf::Vector2f getSecondaryTitlePosition(sf::FloatRect secondaryTitle, sf::FloatRect primaryTitle);

	void initializeLives();
	void initializeScore();
	void initializeGameOverText();
	void initializeExtraLivesText();
	void initializeHealthBar();
	void initializeHealthBarSegment();
	bool isHealthBarSegmentInitialized() const;
	void initializePlayerHealth();
	void initializeShipPointValue();
	

	std::vector<Ship> m_lives;
	std::vector<sf::RectangleShape> m_healthBarSegments;
	std::vector<TempText> m_texts;
	std::vector<sf::Text> m_gameOverTexts;
	std::vector<sf::Text> m_pauseTexts;
	std::vector<sf::Text> m_mainMenuTexts;

	sf::Text m_scoreText;
	sf::Text m_extraLivesText;
	sf::Vector2f m_baseScale;
	sf::RectangleShape m_healthBarSegment;
	float m_windowMargin{};
	Ship m_lifeSymbol;
	int m_totalExtraLives{};
	BoundedFloatRect m_windowDimensions;
	TempText m_introPrimaryText;
	TempText m_introSecondaryText;
	TempText m_shipPointValue;
	bool m_isUiHidden;

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};


