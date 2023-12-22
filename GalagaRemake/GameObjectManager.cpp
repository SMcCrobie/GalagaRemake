#include "GameObjectManager.h"

#include "GameState.h"
#include "UIManager.h"

void GameObjectManager::createGameObject(const GameObject& obj)
{
	m_gameObjects.emplace_back(obj);
}

void GameObjectManager::createItem(const Item& item)
{
	m_items.emplace_back(item);
}

void GameObjectManager::createCollidable(Collidable& collidable)
{
	m_collidables.emplace_back(collidable);
}

bool GameObjectManager::isItemsEmpty() const
{
	return m_items.empty();
}

size_t GameObjectManager::itemsCount() const
{
	return m_items.size();
}

bool GameObjectManager::isCollidablesEmpty() const
{
	return m_collidables.empty();
}

size_t GameObjectManager::collidablesCount() const
{
	return m_collidables.size();
}

void GameObjectManager::resetManager()
{
	m_gameObjects.clear();
	m_items.clear();
	m_collidables.clear();
}

void GameObjectManager::updateCollidables()
{
	for (auto it = m_collidables.begin(); it != m_collidables.end(); ) {
		it->update();
		auto collisionResult = it->detectProjectileCollision();
		if(collisionResult.has_value())
		{
			it->applyPhysicsFromProjectile(collisionResult.value());
			if(it->getHealth() < 1)
			{
				it = m_collidables.erase(it);
				continue;
			}

		}

		for (auto innerIt = it; innerIt != m_collidables.end(); )
		{
			innerIt++;
			if (innerIt == m_collidables.end())
				break;
			auto pointOfImpact = it->detectCollision(*innerIt);
			if (pointOfImpact.has_value())
			{
				it->applyPhysicsToEachOther(*innerIt, pointOfImpact.value());
				if (innerIt->getHealth() < 1) {
					innerIt = m_collidables.erase(innerIt);
				}
				if (it->getHealth() < 1) {
					break;
				}
			}
		}
		if (it != m_collidables.end() && it->getHealth() < 1)
		{
			it = m_collidables.erase(it);
			continue;
		}
		if(checkIfOutOfBounds(*it))
		{
			it = m_collidables.erase(it);
		}else
		{
			it++;
		}
	}
	
}

void GameObjectManager::updateItems()
{
	for (auto& item : m_items)
	{
		item.update();
	}
}

void GameObjectManager::updateGameObjects()
{
	for (auto& gameObject : m_gameObjects)
	{
		gameObject.update();
	}
}

void GameObjectManager::update()
{
	updateGameObjects();
	updateItems();
	updateCollidables();

}

bool GameObjectManager::checkIfOutOfBounds(const Collidable& collidable)
{
	const auto bounds = GameState::world_bounds.scaleWithMargins(150.f);
	if (bounds.contains(collidable.getPosition()))
		return false;
	return true;
}

std::optional<ItemType> GameObjectManager::detectCollisionWithItems(const PlayerShip& playerShip)
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
	return std::nullopt;
}

void GameObjectManager::addItemPointValueToScore(const Item& item, const PlayerShip& playerShip)
{
	extern UIManager uiManager;
	if (item.getItemType() == ItemType::Repair_Kit)
	{
		const auto health = playerShip.getHealth();
		if(health == 5)
			uiManager.addPointValue(item.getPosition(), 800, sf::Color(0x05ecf1ff), 1.0f);
		else
			uiManager.addPointValue(item.getPosition(), health * 100);
	}
}


void GameObjectManager::detectItemCollision(PlayerShip& playerShip)
{
	for (auto it = m_items.begin(); it != m_items.end();)
	{
		if (it->detectCollision(playerShip))
		{
			addItemPointValueToScore(*it, playerShip);
			playerShip.useItem(it->getItemType());
			it = m_items.erase(it);
		}
		if (it == m_items.end())
			break;
		it++;
	}
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
	for (const auto& collidable : m_collidables)
	{
		target.draw(collidable, states);
	}
}
