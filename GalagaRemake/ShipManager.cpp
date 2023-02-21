#include "ShipManager.h"

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
		it->first.updateShipVelocity(worldBounds);
		it->first.move();

		if (it->first.getGlobalBounds().top > worldBounds.bottom) {
			auto tempPosition = it->first.getGlobalBounds();
			it->first.setPosition(tempPosition.left + tempPosition.width//requried due to origin being bottom right with rotation at 180
				, worldBounds.top - 100.f);
		}

	}
}

void ShipManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = m_ships.begin(); it != m_ships.end(); it++) {
		target.draw(it->first);
	}
}
