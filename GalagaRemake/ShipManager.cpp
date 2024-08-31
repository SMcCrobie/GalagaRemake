#include "ShipManager.h"
#include <iostream>

#include "Fonts.h"
#include "GameState.h"
#include "TempText.h"
#include "UIManager.h"

void ShipManager::createShip(const Ship& ship)
{
	m_ships.emplace_back(ship, StateMachineController());
}

void ShipManager::createShip(const Ship& ship, const StateMachineController& controller)
{
	m_ships.emplace_back(ship, controller);
}

void ShipManager::updateShips()
{
	for (auto&[ship, controller] : m_ships)
	{
		controller.updateControllerStateAndShipState(ship);
		ship.updateShip(GameState::world_bounds);

		const BoundedFloatRect shipBounds = ship.getGlobalBounds();
		if (shipBounds.top > GameState::world_bounds.bottom && ship.isBackwards()) {
			ship.rotate180();
		}

		if (shipBounds.bottom < GameState::world_bounds.top && !ship.isBackwards()) {
			ship.rotate180();
		}

	}
}

void ShipManager::detectCollision(ProjectileManager& projectileManager)
{
	extern UIManager uiManager;
	extern SoundManager soundManager;
	auto it = m_ships.begin();
	while (it != m_ships.end()) {
		it->first.detectCollision(projectileManager);
		if(it->first.hasHealth())
		{
			++it;
			continue;
		}
		const auto pointValue = it->first.getPointValue();
		GameState::killCounter++;
		if (pointValue >= 300)
		{
			uiManager.addPointValue(it->first.getPosition(), it->first.getPointValue(), sf::Color::Red, .9f);
		}
		else
		{
			uiManager.addPointValue(it->first.getPosition(), it->first.getPointValue());
		}
		soundManager.playSound(it->first.m_deathSoundType);
		it = m_ships.erase(it);//increments the iterator
		std::cout << "Destroy Ship!" << std::endl;
		
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

size_t ShipManager::count() const
{
	return m_ships.size();
}

void ShipManager::resetManager()
{
	m_ships.clear();
}

void ShipManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& [ship, stateMachine] : m_ships)
	{
		target.draw(ship);
		if (ship.hasShield())
		{
			target.draw(ship.m_shield);
		}
			
	}
}
