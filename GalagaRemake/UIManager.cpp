#include "UIManager.h"


#define ELEMENT_BUFFER 5.F

UIManager::UIManager(const PlayerShip& playerShip, const sf::Font& font,
                     const BoundedFloatRect& windowDimensions, int totalExtraLives, float windowMargin)
	: m_font(font), m_baseScale(sf::Vector2f(.45f, .55f)), 
	  m_windowDimensions(windowDimensions), m_windowMargin(windowMargin), m_playerHealth(playerShip.getHealth())
{
	initializeScore();
	initializeGameOverText();
	initializeExtraLivesText();
	initializePlayerHealth(playerShip);
	initializeLives(playerShip, totalExtraLives);
}

void UIManager::addUiText(TempText text)
{
	m_texts.push_back(text);
}


void UIManager::gameOver()
{
	m_isGameOver = true;
}

bool UIManager::isOutOfLives()
{
	return m_lives.empty();
}

void UIManager::playerLostLife()
{
	m_lives.back().respawnShip(101);
	m_playerDied = true;
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
	if(m_playerHealth != m_healthBarSegments.size() && !m_healthBarSegments.empty())
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
	if (!m_lives.back().isRespawning() && !m_playerDied)
		return;
	if (!m_lives.back().isRespawning() && m_playerDied) {
		m_playerDied = false;
		m_lives.pop_back();
		reinitializePlayerHealth();
	}
	else {
		m_lives.back().updateRespawnTimer();
	}

}

void UIManager::initializeLevelIntroText(TempText& primaryText, TempText& secondaryText)
{
	primaryText.setScale(1.5f, 1.5f);
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

sf::Vector2f UIManager::centerElement(const sf::FloatRect rect) const
{
	const float xPos = (m_windowDimensions.width - rect.width) / 2;
	const float yPos = (m_windowDimensions.height / 2.5f) - rect.height;

	return {xPos, yPos};
}

void UIManager::initializeLives(const Ship& livesShipRepresentation, int totalExtraLives)
{
	m_lives.push_back(livesShipRepresentation);
	auto& lifeSymbol = m_lives.back();
	lifeSymbol.setScale(.4f, .4f);
	auto temp = lifeSymbol.getTextureRect();
	temp.left = temp.width;
	lifeSymbol.setTextureRect(temp);

	sf::FloatRect extraLivesTextSize = m_extraLivesText.getGlobalBounds();
	sf::FloatRect lifeSymbolSize = lifeSymbol.getGlobalBounds();
	float xPos = extraLivesTextSize.left + extraLivesTextSize.width + ELEMENT_BUFFER;
	float yPos = m_windowDimensions.top + m_windowDimensions.height - m_windowMargin - (lifeSymbolSize.height);//extraLivesTextSize.top + ((extraLivesTextSize.height - lifeSymbolSize.height) / 2);
	float lifeXOffest = lifeSymbolSize.width + ELEMENT_BUFFER;

	lifeSymbol.setPosition(xPos, yPos);
	lifeSymbol.setVelocity(0, 0);
	lifeSymbol.setStatic();
	for (int i = 1; i < totalExtraLives; i++) {
		m_lives.push_back(m_lives[0]);
		m_lives[i].move(lifeXOffest *(float)i, 0.f);
	}
}

void UIManager::initializeScore()
{
	m_scoreText = sf::Text("Score  0", m_font);

	const float xPos = m_windowDimensions.left + m_windowMargin;
	const float yPos = m_windowDimensions.top + m_windowMargin;

	m_scoreText.setPosition(xPos, yPos);
	m_scoreText.setScale(m_baseScale);
	//m_scoreText.setFillColor(sf::Color(0x05ecf1ff));
}

void UIManager::initializeGameOverText()
{
	m_gameOverText = sf::Text("Game Over", m_font);
	m_gameOverText.setScale(m_baseScale * 4.61f);

	const BoundedFloatRect textSize = m_gameOverText.getGlobalBounds();
	const float xPos = (m_windowDimensions.width - textSize.width) / 2;
	const float yPos = (m_windowDimensions.height / 2.5f) - textSize.height;

	m_gameOverText.setPosition(xPos, yPos);
	m_gameOverText.setFillColor(sf::Color(0x05ecf1ff));
}

void UIManager::initializeExtraLivesText()
{
	m_extraLivesText = sf::Text("Lives ", m_font);
	m_extraLivesText.setScale(m_baseScale);

	const sf::FloatRect textSize = m_extraLivesText.getGlobalBounds();
	const float xPos = m_windowDimensions.left + m_windowMargin;
	const float yPos = m_windowDimensions.top + m_windowDimensions.height
						- m_windowMargin - textSize.height - 4.f;//needed since the text height doesnt correctly match shown text height

	m_extraLivesText.setPosition(xPos, yPos);
	m_extraLivesText.setFillColor(sf::Color(0x05ecf1ff));
}

void UIManager::initializeHealthSegments()
{
	const float xPos = m_windowDimensions.left + m_windowDimensions.width - m_healthBarSegment.getGlobalBounds().width - m_windowMargin;
	const float yPos = m_windowDimensions.top + m_windowDimensions.height - m_windowMargin - m_healthBarSegment.getGlobalBounds().height;

	m_healthBarSegment.setPosition(xPos, yPos);

	for(int i = 0; i < m_playerHealth; i++)
	{
		m_healthBarSegments.push_back(m_healthBarSegment);
		m_healthBarSegment.move(-m_healthBarSegment.getGlobalBounds().width - ELEMENT_BUFFER, 0.f);
	}
}

void UIManager::initializeHealthBarSegment()
{
	sf::RectangleShape healthBarSegment;
	healthBarSegment.setFillColor(sf::Color::White);
	healthBarSegment.setSize(sf::Vector2f(30.f, 6.f));
	/*healthBarSegment.setOutlineColor(sf::Color(0xffffff4c));
	healthBarSegment.setOutlineThickness(4.f);*/

	m_healthBarSegment = healthBarSegment;
}

void UIManager::initializePlayerHealth(const PlayerShip& playerShip)
{
	initializeHealthBarSegment();
	initializeHealthSegments();

}

void UIManager::reinitializePlayerHealth()
{
	initializeHealthSegments();
}



void UIManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_scoreText);
	target.draw(m_extraLivesText);
	if (m_isGameOver)
		target.draw(m_gameOverText);
	for (auto& life : m_lives) 
		target.draw(life);
	for (auto& text : m_texts) 
		target.draw(text);
	for (auto& healthBarSegment : m_healthBarSegments)
		target.draw(healthBarSegment);
}
