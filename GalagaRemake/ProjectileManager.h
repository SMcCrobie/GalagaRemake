#pragma once
#include "Ship.h"
#include "Projectile.h"
#include <list>


class ProjectileManager 
	: public sf::Drawable
{
public:
	ProjectileManager();
	void collectProjectile(Ship& ship);
	void updateProjectiles(const BoundedFloatRect& worldBounds);

private:
	friend Projectile;

	std::list<Projectile> m_projectiles;
	

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

