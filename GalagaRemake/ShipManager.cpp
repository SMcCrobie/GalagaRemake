#include "ShipManager.h"
#include <iostream>


ShipManager::ShipManager() :
	m_ships(),
	m_shipController()
{
}

void ShipManager::createShip(Ship& ship)
{
	m_ships.push_back({ ship, StateMachineController() });
}

void ShipManager::updateShips(const BoundedFloatRect& worldBounds, const sf::Clock& clock)
{
	for (auto it = m_ships.begin(); it != m_ships.end(); it++) {
		it->second.updateControllerStateAndShipState(clock, it->first);
		it->first.setTextureRectBasedOnShipState();
		it->first.updateShip(worldBounds);
		it->first.moveShip();

		if (it->first.getGlobalBounds().top > worldBounds.bottom && it->first.isBackwards()) {
			auto tempPosition = it->first.getGlobalBounds();
			it->first.rotate180();
		}

	}
}

void ShipManager::detectCollision(ProjectileManager& projectileManager, int& killCounter)
{
	auto it = m_ships.begin();
	while (it != m_ships.end()) {
		if (projectileManager.detectCollisionAndDestroyProjectile(it->first.getGlobalBounds())) {
			it = m_ships.erase(it);//increments the iterator
			std::cout << "Destroy Ship!" << std::endl;
			killCounter++;
		}
		else {
			it++;
		}
	}
}

void ShipManager::offloadProjectiles(ProjectileManager& projectileManager)
{
	for (auto it = m_ships.begin(); it != m_ships.end(); it++) {
		projectileManager.collectProjectile(it->first);
	}
}

void ShipManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = m_ships.begin(); it != m_ships.end(); it++) {
		target.draw(it->first);
	}
}
