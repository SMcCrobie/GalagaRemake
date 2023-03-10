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
	bool detectCollision(const sf::FloatRect& gameObject);
	bool detectCollisionAndDestroyProjectile(const sf::FloatRect& gameObject);
	std::list<RectangleProjectile>::iterator findProjectileInCollision(const sf::FloatRect& gameObject);

private:
	std::list<RectangleProjectile> m_projectiles;

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};