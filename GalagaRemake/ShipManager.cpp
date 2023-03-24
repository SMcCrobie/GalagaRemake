#include "ShipManager.h"
#include <iostream>

void ShipManager::createShip(Ship& ship)
{

	m_ships.emplace_back(ship, StateMachineController());
}

void ShipManager::createShip(Ship& ship, StateMachineController& controller)
{
	m_ships.emplace_back(ship, controller);
}

void ShipManager::updateShips(const BoundedFloatRect& worldBounds, const sf::Clock& clock)
{
	for (auto it = m_ships.begin(); it != m_ships.end(); it++) {
		it->second.updateControllerStateAndShipState(clock, it->first);
		it->first.setTextureRectBasedOnShipState();
		it->first.updateShip(worldBounds);
		//it->first.moveShip();

		const BoundedFloatRect shipBounds = it->first.getGlobalBounds();
		if (shipBounds.top > worldBounds.bottom && it->first.isBackwards()) {
			it->first.rotate180();
		}

		if (shipBounds.bottom < worldBounds.top && !it->first.isBackwards()) {
			it->first.rotate180();
		}

	}
}

void ShipManager::detectCollision(ProjectileManager& projectileManager, int& killCounter)
{
	auto it = m_ships.begin();
	while (it != m_ships.end()) {
		it->first.detectCollision(projectileManager);
		if(it->first.hasHealth())
		{
			++it;
			continue;
		}

		it = m_ships.erase(it);//increments the iterator
		std::cout << "Destroy Ship!" << std::endl;
		killCounter++;
	}
}

void ShipManager::offloadProjectiles(ProjectileManager& projectileManager)
{
	for (auto it = m_ships.begin(); it != m_ships.end(); it++) {
		projectileManager.collectProjectile(it->first);
	}
}

bool ShipManager::isEmpty() const
{
	return m_ships.empty();
}

void ShipManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = m_ships.begin(); it != m_ships.end(); it++) {
		target.draw(it->first);
		if (it->first.hasShield())
		{
			target.draw(it->first.m_shield);
		}
			
	}
}
