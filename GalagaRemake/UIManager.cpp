#include "UIManager.h"
#include "GameState.h"
#include "MultiColorText.h"

#include <format>

constexpr float element_buffer = 5.f;
constexpr float health_bar_max_width = 150.f;
constexpr int life_score_value = 1000;
constexpr int health_segment_score_value = 100;


extern PlayerShip playerShip;



void UIManager::initializePauseText()
{
	auto primaryText = sf::Text("Game Paused", Fonts::galaxus);
	formatPrimaryText(primaryText);
	primaryText.setFillColor(PRIMARY_COLOR);
	m_pauseTexts.push_back(primaryText);

	auto templateText = sf::Text("", Fonts::play_fair);

	auto pauseHelpText = MultiColorText(templateText);
	pauseHelpText.addTextSegment("Press [", PRIMARY_COLOR)
		.addTextSegment("tab", SECONDARY_COLOR, 1.f, true)
		.addTextSegment("] to resume", PRIMARY_COLOR);
	formatSecondaryText(pauseHelpText, primaryText.getGlobalBounds());
	pauseHelpText.dumpSegments(m_pauseTexts);


	auto windowResizeHelpText = MultiColorText(templateText);
	windowResizeHelpText.addTextSegment("Press [", PRIMARY_COLOR)
		.addTextSegment("r", SECONDARY_COLOR, 2.5f, true)
		.addTextSegment("] to reset window size", PRIMARY_COLOR);
	formatSecondaryText(windowResizeHelpText, pauseHelpText.getGlobalBounds());
	windowResizeHelpText.dumpSegments(m_pauseTexts);

}

void UIManager::init(const Ship& shipModel, int totalExtraLives, float windowMargin)
{
	m_baseScale = sf::Vector2f(.45f, .55f);
	m_windowMargin = windowMargin;
	m_lifeSymbol = shipModel;
	m_totalExtraLives = totalExtraLives;
	m_windowDimensions = GameState::world_bounds;


	initializePauseText();
	initializeScore();
	initializeGameOverText();
	initializeExtraLivesText();
	initializePlayerHealth();
	initializeLives();
	initializeShipPointValue();
}

void UIManager::addUiText(const TempText text)
{
	m_texts.push_back(text);
}


bool UIManager::isOutOfLives() const
{
	return m_lives.empty();
}

void UIManager::playerLostLife()
{
	m_lives.back().respawnShip(101);
	GameState::isPlayerDead = true;
}

void UIManager::updateTempText()
{
	if (m_texts.empty())
		return;

	auto it = m_texts.begin();
	while(it != m_texts.end())
	{
		if(it->isDone())
		{
			it = m_texts.erase(it);
			continue;
		}
		it->updateText();
		it++;
	}
}

void UIManager::updateHealthBar()
{
	if(playerShip.getHealth() > m_healthBarSegments.size())
	{
		initializePlayerHealth();
	}
	while(playerShip.getHealth() < m_healthBarSegments.size() && !m_healthBarSegments.empty())
	{
		m_healthBarSegments.pop_back();
	}
}

void UIManager::updateUI()
{
	updateTempText();
	updateHealthBar();
	m_scoreText.setString("Score  " + std::to_string(GameState::score));
	if (m_lives.empty())
		return;
	if (!m_lives.back().isRespawning() && !GameState::isPlayerDead)
		return;
	if (!m_lives.back().isRespawning() && GameState::isPlayerDead) {
		GameState::isPlayerDead = false;
		m_lives.pop_back();
		initializePlayerHealth();
	}
	else {
		m_lives.back().updateRespawnTimer();
	}

}

void UIManager::formatPrimaryText(sf::Text& primaryText)
{
	primaryText.setCharacterSize(46);
	const sf::Vector2f windowPosition = getPrimaryTitlePosition(primaryText.getGlobalBounds());
	primaryText.setPosition(windowPosition);

}

void UIManager::formatSecondaryText(sf::Text& secondaryText, const sf::FloatRect primaryText)
{
	secondaryText.setCharacterSize(18);
	const sf::Vector2f windowPosition = getSecondaryTitlePosition(secondaryText.getGlobalBounds(), primaryText);
	secondaryText.setPosition(windowPosition);
}


template <typename TextType>
void UIManager::formatSecondaryText(MultiColorText<TextType>& secondaryText, const sf::FloatRect& primaryText)
{
	secondaryText.setCharacterSize(16);
	const sf::Vector2f windowPosition = getSecondaryTitlePosition(secondaryText.getGlobalBounds(), primaryText);
	secondaryText.setPosition(windowPosition);
}


void UIManager::initializeShipPointValue()
{
	m_shipPointValue = TempText("100", Fonts::play_fair);
	m_shipPointValue.setDuration(80);
	m_shipPointValue.setFadeOut(80);
	m_shipPointValue.setScale(.6f, .6f);
}


void UIManager::initializeLevelIntroText(TempText& primaryText, TempText& secondaryText)
{
	primaryText.setDuration(200);
	primaryText.setFadeOut(80);
	primaryText.setFillColor(SECONDARY_COLOR);
	formatPrimaryText(primaryText);


	secondaryText.setDuration(200);
	secondaryText.setFadeOut(80);
	formatSecondaryText(secondaryText, primaryText.getGlobalBounds());


	m_texts.push_back(primaryText);
	m_texts.push_back(secondaryText);
}

void UIManager::initializeLevelOutroText(TempText& primaryText, TempText& secondaryText)
{
	primaryText.setDuration(1000);
	primaryText.setFadeOut(80);
	primaryText.setFillColor(SECONDARY_COLOR);
	formatPrimaryText(primaryText);

	secondaryText.setDuration(200);
	secondaryText.setFadeOut(80);
	formatSecondaryText(secondaryText, primaryText.getGlobalBounds());



	m_texts.push_back(primaryText);
	m_texts.push_back(secondaryText);


	TempText lifeScoreTemplateText("", Fonts::galaxus);
	GameState::score += life_score_value * static_cast<int>(m_lives.size());
	GameState::score += health_segment_score_value * playerShip.getHealth();

	lifeScoreTemplateText.setDelay(200);
	lifeScoreTemplateText.setDuration(800);
	lifeScoreTemplateText.setFadeOut(80);
	lifeScoreTemplateText.setCharacterSize(32);
	
	auto lifeScoreTally = MultiColorText(lifeScoreTemplateText);
	lifeScoreTally.addTextSegment("Extra Lives: [", PRIMARY_COLOR)
		.addTextSegment(std::to_string(m_lives.size()), SECONDARY_COLOR)
		.addTextSegment("] " + std::to_string(life_score_value) + " = ", PRIMARY_COLOR)
		.addTextSegment("+" + std::to_string(life_score_value * m_lives.size()), SECONDARY_COLOR);

	formatSecondaryText(lifeScoreTally, primaryText.getGlobalBounds());
	lifeScoreTally.dumpSegments(m_texts);


	auto healthScoreTally = MultiColorText(lifeScoreTemplateText);
	healthScoreTally.addTextSegment("Extra Health: [", PRIMARY_COLOR)
		.addTextSegment(std::to_string(playerShip.getHealth()), SECONDARY_COLOR)
		.addTextSegment("] " + std::to_string(health_segment_score_value) + " = ", PRIMARY_COLOR)
		.addTextSegment("+" + std::to_string(health_segment_score_value * playerShip.getHealth()), SECONDARY_COLOR);

	formatSecondaryText(healthScoreTally, lifeScoreTally.getGlobalBounds());
	healthScoreTally.dumpSegments(m_texts);
}








void UIManager::resetManager()
{
	initializePlayerHealth();
	initializeLives();

	m_texts.clear();
	m_texts.push_back(m_introPrimaryText);
	m_texts.push_back(m_introSecondaryText);
}

void UIManager::addPointValue(sf::Vector2f position, int pointValue, sf::Color color, float scale)
{
	GameState::score += pointValue;


	m_texts.push_back(m_shipPointValue);
	TempText& pointsText = m_texts.back();
	pointsText.setString(std::to_string(pointValue));
	pointsText.setOrigin(m_shipPointValue.getLocalBounds().width / 2.f, m_shipPointValue.getLocalBounds().height / 2.f);
	pointsText.setPosition(position);
	pointsText.setFillColor(color);
	if (scale > 0.f)
		pointsText.setScale(scale, scale);
}


sf::Vector2f UIManager::getPrimaryTitlePosition(const sf::FloatRect rect)
{
	const float xPos = CENTER_ELEMENT_HORIZONTALLY(rect.width);
	const float yPos = CENTER_ELEMENT_VERTICALLY(rect.height) - 100;

	return {xPos, yPos};
}

sf::Vector2f UIManager::getSecondaryTitlePosition(sf::FloatRect secondaryTitle, sf::FloatRect primaryTitle)
{
	const float xPos = CENTER_ELEMENT_HORIZONTALLY(secondaryTitle.width);
	const float yPos = primaryTitle.top + primaryTitle.height + element_buffer;

	return { xPos, yPos };
}


void UIManager::initializeLives()
{
	m_lives.clear();
	m_lives.push_back(m_lifeSymbol);
	auto& lifeSymbol = m_lives.back();
	lifeSymbol.setOrigin(0.f, 0.f);

	lifeSymbol.setScale(.4f, .4f);
	sf::IntRect temp = lifeSymbol.getTextureRect();
	temp.left = temp.width;
	lifeSymbol.setTextureRect(temp);

	const sf::FloatRect extraLivesTextSize = m_extraLivesText.getGlobalBounds();
	const sf::FloatRect lifeSymbolSize = lifeSymbol.getGlobalBounds();
	const float xPos = extraLivesTextSize.left + extraLivesTextSize.width + element_buffer;
	const float yPos = m_windowDimensions.top + m_windowDimensions.height - m_windowMargin - (lifeSymbolSize.height);
	const float lifeXOffset = lifeSymbolSize.width + element_buffer;

	lifeSymbol.setPosition(xPos, yPos);
	lifeSymbol.setVelocity(0, 0);
	lifeSymbol.setStatic();
	for (int i = 1; i < m_totalExtraLives; i++) {
		m_lives.push_back(m_lives[0]);
		m_lives[i].move(lifeXOffset *static_cast<float>(i), 0.f);
	}
}

void UIManager::initializeScore()
{
	m_scoreText = sf::Text("Score  0", Fonts::galaxus);

	const float xPos = m_windowDimensions.left + m_windowMargin;
	const float yPos = m_windowDimensions.top + m_windowMargin;

	m_scoreText.setPosition(xPos, yPos);
	m_scoreText.setScale(m_baseScale);
}

void UIManager::initializeGameOverText()
{
	auto text = sf::Text("Game Over", Fonts::galaxus);
	formatPrimaryText(text);
	m_gameOverTexts.push_back(text);
}

void UIManager::initializeExtraLivesText()
{
	m_extraLivesText = sf::Text("Lives ", Fonts::galaxus);
	m_extraLivesText.setScale(m_baseScale);

	const sf::FloatRect textSize = m_extraLivesText.getGlobalBounds();
	const float xPos = m_windowDimensions.left + m_windowMargin;
	const float yPos = m_windowDimensions.top + m_windowDimensions.height
						- m_windowMargin - textSize.height - 4.f;//needed since the text height doesnt correctly match shown text height

	m_extraLivesText.setPosition(xPos, yPos);
	m_extraLivesText.setFillColor(SECONDARY_COLOR);
}

void UIManager::initializeHealthBar()
{
	
	m_healthBarSegments.clear();
	const float xPos = m_windowDimensions.left + m_windowDimensions.width - m_healthBarSegment.getGlobalBounds().width - m_windowMargin;
	const float yPos = m_windowDimensions.top + m_windowDimensions.height - m_windowMargin - m_healthBarSegment.getGlobalBounds().height;

	m_healthBarSegment.setPosition(xPos, yPos);

	for(int i = 0; i < playerShip.getHealth(); i++)
	{
		m_healthBarSegments.push_back(m_healthBarSegment);
		m_healthBarSegment.move(-m_healthBarSegment.getGlobalBounds().width - element_buffer, 0.f);
	}
}

void UIManager::initializeHealthBarSegment()
{
	sf::RectangleShape healthBarSegment;
	healthBarSegment.setFillColor(PRIMARY_COLOR);

	const auto health = static_cast<float>(playerShip.getHealth());
	const auto healthSegmentWidth = (health_bar_max_width - (element_buffer * (health-1))) / health;

	healthBarSegment.setSize(sf::Vector2f(healthSegmentWidth, 6.f));

	m_healthBarSegment = healthBarSegment;
}

bool UIManager::isHealthBarSegmentInitialized() const
{
	return m_healthBarSegment.getSize().x < 1.f;
}

void UIManager::initializePlayerHealth()
{
	//if (isHealthBarSegmentInitialized())
	initializeHealthBarSegment();
	initializeHealthBar();
}



void UIManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_scoreText);
	target.draw(m_extraLivesText);
	for (auto& life : m_lives) 
		target.draw(life);
	if(!GameState::isPaused && !GameState::isGameOver)
	{
		for (auto& text : m_texts)
			target.draw(text);
	}
	for (auto& healthBarSegment : m_healthBarSegments)
		target.draw(healthBarSegment);


	if (GameState::isGameOver)
	{
		for (const auto& text : m_gameOverTexts)
			target.draw(text);

	}
		
	if (GameState::isPaused && !GameState::isGameOver)
	{
		for (const auto& text : m_pauseTexts)
			target.draw(text);
	}
		
}
