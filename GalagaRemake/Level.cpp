#include "Level.h"

#include "GameState.h"

Level& Level::addDrawableLayer(const std::shared_ptr<sf::Drawable>& drawable)
{
	m_drawables.push_back(drawable);
	return *this;
}

bool Level::checkForGameEvent() const
{
	if (GameState::requiresLevelRestart)
	{
		GameState::init();
		reset();
		return true;
	}
	if (GameState::isGameOver || GameState::isPaused)
		return true;
	return false;
}

Level& Level::addManager(const std::shared_ptr<Manager>& manager)
{
	m_managers.push_back(manager);
	return *this;
}

void Level::reset() const
{
	for(const auto& manager : m_managers)
	{
		manager->resetManager();
	}
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& drawable : m_drawables)
	{
		target.draw(*drawable);
	}
}
