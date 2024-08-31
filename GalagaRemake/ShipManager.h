#pragma once
#include "Ship.h"

#include "KeyboardController.h"
#include <list>
#include "ProjectileManager.h"

class ShipManager
	: public sf::Drawable, public IManager
{
public:
	void createShip(const Ship& ship);
	void createShip(const Ship& ship, const StateMachineController& controller);
	void updateShips();
	void detectCollision(ProjectileManager& projectileManager);//and getScore
	void offloadProjectiles(ProjectileManager& projectileManager);
	[[nodiscard]] bool isEmpty() const;
	[[nodiscard]] size_t count() const;
	void resetManager() override;

private:
	std::list<std::pair<Ship, StateMachineController>> m_ships;
	StateMachineController m_shipController;//might want a vec later on
	sf::Sound m_soundPlayer;
	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

