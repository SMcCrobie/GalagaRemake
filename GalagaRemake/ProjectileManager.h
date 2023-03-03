#pragma once
#include "Ship.h"
#include "Projectile.h"
#include <list>
#include "ShipManager.h"


class ProjectileManager 
	: public sf::Drawable
{
public:
	ProjectileManager();
	void collectProjectile(Ship& ship);
	void collectProjectile(ShipManager& shipManager);
	void updateProjectiles(const BoundedFloatRect& worldBounds);
	bool detectCollision(const sf::FloatRect& gameObject, bool destroyProjectileInCollision = true);
	void detectCollision(ShipManager& shipManager, int& killCounter);

private:
	std::list<Projectile> m_projectiles;

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

