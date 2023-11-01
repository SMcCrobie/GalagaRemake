#include "GameObjectManager.h"

void GameObjectManager::createGameObject(const GameObject& obj)
{
	m_gameObjects.emplace_back(obj);
}

bool GameObjectManager::isEmpty() const
{
	return m_gameObjects.empty();
}

int GameObjectManager::count() const
{
	return m_gameObjects.size();
}

void GameObjectManager::resetManager()
{
	m_gameObjects.clear();
}

void GameObjectManager::updateObjects()
{
	for (auto& gameObject : m_gameObjects)
	{
		gameObject.updateGameObject();
	}
}

void GameObjectManager::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	for (const auto& gameObject : m_gameObjects)
	{
		target.draw(gameObject, states);
	}
}
