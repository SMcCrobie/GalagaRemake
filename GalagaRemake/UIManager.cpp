#include "UIManager.h"

UIManager::UIManager(const Ship& livesShipRepresentation, const sf::Font& font, 
	BoundedFloatRect windowDimensions, int totalExtraLives, float windowMargin)
	: m_font(font), m_baseScale(sf::Vector2f(.45f, .55f)), 
	  m_windowDimensions(windowDimensions), m_windowMargin(windowMargin)
{
	initializeScore();
	initializeGameOverText();
	initializeExtraLivesText();
	initializeLives(livesShipRepresentation, totalExtraLives);
}

sf::Text& UIManager::addUiText(std::string text)
{
	m_texts.push_back(sf::Text(text, m_font));
	return m_texts.back();
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
	m_lives.back().respawnShip();
	m_playerDied = true;
}

void UIManager::updateUI(int currentScore)
{
	m_scoreText.setString("Score  " + std::to_string(currentScore));
	if (m_lives.empty())
		return;
	if (!m_lives.back().isRespawning() && !m_playerDied)
		return;
	if (!m_lives.back().isRespawning() && m_playerDied) {
		m_playerDied = false;
		m_lives.pop_back();
	}
	else {
		m_lives.back().updateRespawnTimer();
	}

}

void UIManager::initializeLives(const Ship& livesShipRepresentation, int totalExtraLives)
{
	m_lives.push_back(livesShipRepresentation);
	auto& lifeSymbol = m_lives.back();
	lifeSymbol.setScale(.4f, .4f);
	lifeSymbol.rotate(30.f);

	sf::FloatRect extraLivesTextSize = m_extraLivesText.getGlobalBounds();
	sf::FloatRect lifeSymbolSize = lifeSymbol.getGlobalBounds();
	float xPos = extraLivesTextSize.left + extraLivesTextSize.width + (extraLivesTextSize.width / 8);
	float yPos = extraLivesTextSize.top + ((extraLivesTextSize.height - lifeSymbolSize.height) / 2);
	float lifeXOffest = lifeSymbolSize.width;

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
	
	float xPos = m_windowDimensions.left + m_windowMargin;
	float yPos = m_windowDimensions.top + m_windowMargin;

	m_scoreText.setPosition(xPos, yPos);
	m_scoreText.setScale(m_baseScale);
	m_scoreText.setFillColor(sf::Color(0x05ecf1ff));
}

void UIManager::initializeGameOverText()
{
	m_gameOverText = sf::Text("Game Over", m_font);
	m_gameOverText.setScale(m_baseScale * 4.61f);
	
	BoundedFloatRect textSize = m_gameOverText.getGlobalBounds();
	float xPos = (m_windowDimensions.width - textSize.width) / 2;
	float yPos = (m_windowDimensions.height / 2.5f) - textSize.height;

	m_gameOverText.setPosition(xPos, yPos);
	m_gameOverText.setFillColor(sf::Color(0x05ecf1ff));
}

void UIManager::initializeExtraLivesText()
{
	m_extraLivesText = sf::Text("Extra Lives ", m_font);
	m_extraLivesText.setScale(m_baseScale);

	sf::FloatRect textSize = m_extraLivesText.getGlobalBounds();
	float xPos = m_windowDimensions.left + m_windowMargin;
	float yPos = m_windowDimensions.top + m_windowDimensions.height - m_windowMargin - textSize.height \
		- 4.f;//needed since the text height doesnt correctly match shown text height

	m_extraLivesText.setPosition(xPos, yPos);
	m_extraLivesText.setFillColor(sf::Color(0x05ecf1ff));
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
}
