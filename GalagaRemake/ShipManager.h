#pragma once
#include "Ship.h"

#include "Controller.h"
#include <list>
#include "ProjectileManager.h"

class ShipManager
	: public sf::Drawable
{
public:
	void createShip(const Ship& ship);
	void createShip(const Ship& ship, const StateMachineController& controller);
	void updateShips(const BoundedFloatRect& worldBounds, const sf::Clock& clock);
	void detectCollision(ProjectileManager& projectileManager, int& killCounter);
	void offloadProjectiles(ProjectileManager& projectileManager);
	bool isEmpty() const;

private:
	std::list<std::pair<Ship, StateMachineController>> m_ships;
	StateMachineController m_shipController;//might want a vec later on

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

