#include "UIManager.h"
#include "GameState.h"


#define CENTER_ELEMENT_HORIZONTALLY(elementWidth) ((m_windowDimensions.width - (elementWidth)) / 2)
#define ELEMENT_BUFFER 5.F
#define HEALTH_BAR_MAX_WIDTH 150.F

extern PlayerShip playerShip;

void UIManager::initializePauseText()
{
	m_pauseText = sf::Text("Game Paused", Fonts::galaxus);
	stylePrimaryText(m_pauseText);
	m_pauseText.setFillColor(sf::Color(255,255,255,255));

	m_pauseTextByline = sf::Text("Press [       ] to resume", Fonts::playFair);
	styleSecondaryText(m_pauseTextByline);

	m_pauseTextByline2 = sf::Text("tab", Fonts::playFair);
	styleSecondaryText(m_pauseTextByline2);
	m_pauseTextByline2.move(-16.f, -1.f);
	m_pauseTextByline2.setOutlineColor(sf::Color(0x05ecf1ff));
	m_pauseTextByline2.setFillColor(sf::Color(0x05ecf1ff));
	m_pauseTextByline2.setOutlineThickness(1.f);


}

UIManager::UIManager() = default;


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
}

void UIManager::addUiText(TempText text)
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
	extern PlayerShip playerShip;
	if(playerShip.getHealth() > m_healthBarSegments.size())
	{
		initializePlayerHealth();
	}
	while(playerShip.getHealth() < m_healthBarSegments.size() && !m_healthBarSegments.empty())
	{
		m_healthBarSegments.pop_back();
	}
}

void UIManager::updateUI(int currentScore)
{
	updateTempText();
	updateHealthBar();
	m_scoreText.setString("Score  " + std::to_string(currentScore));
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

void UIManager::stylePrimaryText(sf::Text& primaryText)
{
	primaryText.setScale(1.5f, 1.5f);
	primaryText.setFillColor(sf::Color(0x05ecf1ff));
	sf::Vector2f windowPosition = centerElement(primaryText.getGlobalBounds());
	primaryText.setPosition(windowPosition);
}

void UIManager::styleSecondaryText(sf::Text& secondaryText)
{
	secondaryText.setScale(.5f, .5f);
	sf::Vector2f windowPosition = centerElement(secondaryText.getGlobalBounds());
	windowPosition.y += 30.f;
	secondaryText.setPosition(windowPosition);
}


void UIManager::initializeLevelIntroText(TempText& primaryText, TempText& secondaryText)
{
	//sf::Vector2f windowPosition = centerElement(primaryText.getGlobalBounds());
	stylePrimaryText(primaryText);
	primaryText.setDuration(200);
	primaryText.addFadeOut(80);
	m_introPrimaryText = primaryText;
	m_texts.push_back(primaryText);


	styleSecondaryText(secondaryText);
	secondaryText.setDuration(200);
	secondaryText.addFadeOut(80);
	m_introSecondaryText = secondaryText;
	m_texts.push_back(secondaryText);
}

void UIManager::initializeLevelOutroText(TempText& primaryText, TempText& secondaryText)
{
	primaryText.setScale(1.4f, 1.4f);
	primaryText.setFillColor(sf::Color(0x05ecf1ff));
	sf::Vector2f windowPosition = centerElement(primaryText.getGlobalBounds());
	primaryText.setPosition(windowPosition);
	primaryText.setDuration(200);
	primaryText.addFadeOut(80);
	m_texts.push_back(primaryText);


	secondaryText.setScale(.5f, .5f);
	windowPosition = centerElement(secondaryText.getGlobalBounds());
	windowPosition.y += 30.f;
	secondaryText.setPosition(windowPosition);
	secondaryText.setDuration(200);
	secondaryText.addFadeOut(80);
	m_texts.push_back(secondaryText);

}

void UIManager::resetManager()
{
	initializePlayerHealth();
	initializeLives();

	m_texts.clear();
	m_texts.push_back(m_introPrimaryText);
	m_texts.push_back(m_introSecondaryText);
}

sf::Vector2f UIManager::centerElement(const sf::FloatRect rect) const
{
	const float xPos = CENTER_ELEMENT_HORIZONTALLY(rect.width);
	const float yPos = (m_windowDimensions.height / 2.5f) - rect.height;

	return {xPos, yPos};
}


void UIManager::initializeLives()
{
	m_lives.clear();
	m_lives.push_back(m_lifeSymbol);
	auto& lifeSymbol = m_lives.back();
	lifeSymbol.setOrigin(0.f, 0.f);

	lifeSymbol.setScale(.4f, .4f);
	auto temp = lifeSymbol.getTextureRect();
	temp.left = temp.width;
	lifeSymbol.setTextureRect(temp);

	sf::FloatRect extraLivesTextSize = m_extraLivesText.getGlobalBounds();
	sf::FloatRect lifeSymbolSize = lifeSymbol.getGlobalBounds();
	float xPos = extraLivesTextSize.left + extraLivesTextSize.width + ELEMENT_BUFFER;
	float yPos = m_windowDimensions.top + m_windowDimensions.height - m_windowMargin - (lifeSymbolSize.height);
	float lifeXOffest = lifeSymbolSize.width + ELEMENT_BUFFER;

	lifeSymbol.setPosition(xPos, yPos);
	lifeSymbol.setVelocity(0, 0);
	lifeSymbol.setStatic();
	for (int i = 1; i < m_totalExtraLives; i++) {
		m_lives.push_back(m_lives[0]);
		m_lives[i].move(lifeXOffest *(float)i, 0.f);
	}
}

void UIManager::initializeScore()
{
	m_scoreText = sf::Text("Score  0", Fonts::galaxus);

	const float xPos = m_windowDimensions.left + m_windowMargin;
	const float yPos = m_windowDimensions.top + m_windowMargin;

	m_scoreText.setPosition(xPos, yPos);
	m_scoreText.setScale(m_baseScale);
	//m_scoreText.setFillColor(sf::Color(0x05ecf1ff));
}

void UIManager::initializeGameOverText()
{
	m_gameOverText = sf::Text("Game Over", Fonts::galaxus);
	stylePrimaryText(m_gameOverText);


	m_gameOverByline = sf::Text("Press [      ] to restart", Fonts::playFair);
	styleSecondaryText(m_gameOverByline);

	m_gameOverByline2 = sf::Text("F2", Fonts::playFair);
	styleSecondaryText(m_gameOverByline2);
	m_gameOverByline2.move(-14.5f, -2.f);
	m_gameOverByline2.setOutlineColor(sf::Color(0x05ecf1ff));
	m_gameOverByline2.setFillColor(sf::Color(0x05ecf1ff));
	m_gameOverByline2.setOutlineThickness(1.f);
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
	m_extraLivesText.setFillColor(sf::Color(0x05ecf1ff));
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
		m_healthBarSegment.move(-m_healthBarSegment.getGlobalBounds().width - ELEMENT_BUFFER, 0.f);
	}
}

void UIManager::initializeHealthBarSegment()
{
	sf::RectangleShape healthBarSegment;
	healthBarSegment.setFillColor(sf::Color::White);

	auto health = static_cast<float>(playerShip.getHealth());
	auto healthSegmentWidth = (HEALTH_BAR_MAX_WIDTH - (ELEMENT_BUFFER * (health-1))) / health;

	healthBarSegment.setSize(sf::Vector2f(healthSegmentWidth, 6.f));

	m_healthBarSegment = healthBarSegment;
}

bool UIManager::isHealthBarSegmentInitialized()
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
		target.draw(m_gameOverText);
		target.draw(m_gameOverByline);
		target.draw(m_gameOverByline2);

	}
		
	if (GameState::isPaused && !GameState::isGameOver)
	{
		target.draw(m_pauseText);
		target.draw(m_pauseTextByline);
		target.draw(m_pauseTextByline2);
	}
		
}
