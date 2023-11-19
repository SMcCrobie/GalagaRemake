#pragma once
#include "Ship.h"
#include "Projectile.h"
#include <list>

#include "CollisionResult.h"
#include "IManager.h"

class ProjectileManager 
	: public sf::Drawable, public IManager
{
public:
	void collectProjectile(CircleProjectile& projectile);
	void collectProjectile(Ship& ship);
	void updateProjectiles();
	CollisionResult detectCollisionAndDestroyProjectile(const sf::FloatRect& gameObject);
	CollisionResult detectCollisionAndDestroyProjectile(const sf::Sprite& sprite);
	CollisionResult detectCollisionAndDestroyProjectile(const CircleProjectile& shield);
	void resetManager() override;

private:
	std::list< std::shared_ptr<Projectile>> m_projectiles;

	std::list<std::shared_ptr<Projectile>>::iterator findProjectileInCollision(const sf::FloatRect& gameObject);
	std::list<std::shared_ptr<Projectile>>::iterator findProjectileInCollision(const sf::Sprite& sprite);
	std::list<std::shared_ptr<Projectile>>::iterator findProjectileInCollision(const CircleProjectile& shield);

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
