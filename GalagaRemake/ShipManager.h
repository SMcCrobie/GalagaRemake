#pragma once
#include "Ship.h"
#include "Projectile.h"
#include "Controller.h"
#include <list>

class ShipManager
	: public sf::Drawable
{
public:
	ShipManager();
	void createShip(Ship& ship);
	//void destroyShip(Ship& ship);//might just do this in update function
	void updateShips(const BoundedFloatRect& worldBounds, const sf::Clock& clock);
	

private:
	friend ProjectileManager;
	std::list<std::pair<Ship,StateMachineController>> m_ships;
	StateMachineController m_shipController;//might want a vec later on

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
