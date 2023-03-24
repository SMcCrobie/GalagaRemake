#include <iostream>
#include "ProjectileManager.h"

void ProjectileManager::collectProjectile(CircleProjectile& projectile)
{
	m_projectiles.push_back(projectile.clone());
}

void ProjectileManager::collectProjectile(Ship& ship)
{
	const auto projectile = ship.fireWeapon1IfFired();
	const auto projectile2 = ship.fireWeapon2IfFired();
	if (projectile.has_value())
		m_projectiles.push_back(projectile.value());
	
	if(projectile2.has_value())
		m_projectiles.push_back(projectile2.value());
}

void ProjectileManager::updateProjectiles(const BoundedFloatRect& worldBounds)
{
	auto it = m_projectiles.begin();
	while (it != m_projectiles.end())
	{
		(*it)->updateProjectile();
		sf::FloatRect projectileBounds = (*it)->getGlobalBounds();
		if (worldBounds.intersects(projectileBounds))
			it++;
		else {
			it = m_projectiles.erase(it);
			if (it == m_projectiles.end())
				break;
		}
	}
}

bool ProjectileManager::detectCollision(const sf::FloatRect& gameObject)
{
	return findProjectileInCollision(gameObject) != m_projectiles.end();
}

bool ProjectileManager::detectCollisionAndDestroyProjectile(const sf::FloatRect& gameObject)
{
	auto it = findProjectileInCollision(gameObject);
	if (it == m_projectiles.end())
		return false;
	m_projectiles.erase(it);
	return true;
}

std::list< std::shared_ptr<Projectile>>::iterator ProjectileManager::findProjectileInCollision(const sf::FloatRect& gameObject)
{
	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); it++) {
		if ((*it)->getGlobalBounds().intersects(gameObject)) {
			std::cout << "Collision Detected" << std::endl;
			return it;
		}
	}
	return m_projectiles.end();
}

void ProjectileManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& projectile : m_projectiles) {
		target.draw(*projectile);
	}
}

