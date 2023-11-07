#include "GameObjectManager.h"

void GameObjectManager::createGameObject(const GameObject& obj)
{
	m_gameObjects.emplace_back(obj);
}

void GameObjectManager::createItem(const Item& item)
{
	m_items.emplace_back(item);
}

bool GameObjectManager::isEmpty() const
{
	return m_gameObjects.empty() && m_items.empty();
}

int GameObjectManager::count() const
{
	return m_gameObjects.size() + m_items.size();
}

void GameObjectManager::resetManager()
{
	m_gameObjects.clear();
	m_items.clear();
}

void GameObjectManager::update()
{
	for (auto& gameObject : m_gameObjects)
	{
		gameObject.update();
	}

	for (auto& item : m_items)
	{
		item.update();
	}
}

ItemType GameObjectManager::detectCollisionWithItems(const PlayerShip& playerShip)
{
	for (auto it = m_items.begin(); it != m_items.end(); it++)
	{
		if (it->detectCollision(playerShip))
		{
			const auto type = it->getItemType();
			m_items.erase(it);
			return type;
		}
	}
	return ItemType::None;
}

void GameObjectManager::detectCollision(PlayerShip& playerShip)
{
	//TODO detect collisions with game objects
	playerShip.useItem(detectCollisionWithItems(playerShip));
	
}

void GameObjectManager::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	for (const auto& gameObject : m_gameObjects)
	{
		target.draw(gameObject, states);
	}
	for (const auto& item : m_items)
	{
		target.draw(item, states);
	}
}
