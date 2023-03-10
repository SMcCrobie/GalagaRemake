#pragma once
#include <iostream>
#include "ProjectileManager.h"

ProjectileManager::ProjectileManager()
	: m_projectiles()
{
}

void ProjectileManager::collectProjectile(Ship& ship)
{
	auto projectile = ship.fireWeapon1IfFired();
	if (!projectile.has_value())
		return;

	m_projectiles.push_back(projectile.value());
	return;
}

void ProjectileManager::updateProjectiles(const BoundedFloatRect& worldBounds)
{
	for (std::list<RectangleProjectile>::iterator it = m_projectiles.begin(); it != m_projectiles.end(); it++)
	{
		it->move();
		sf::FloatRect projectileBounds = it->getGlobalBounds();
		if (!worldBounds.intersects(projectileBounds)) {
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

std::list<RectangleProjectile>::iterator ProjectileManager::findProjectileInCollision(const sf::FloatRect& gameObject)
{
	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); it++) {
		if (it->getGlobalBounds().intersects(gameObject)) {
			std::cout << "Collision Detected" << std::endl;
			return it;
		}
	}
	return m_projectiles.end();
}

void ProjectileManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& projectile : m_projectiles) {
		target.draw(projectile);
	}
}

