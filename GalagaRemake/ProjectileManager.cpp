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

void ProjectileManager::collectProjectile(ShipManager& shipManager)
{
	for (auto it = shipManager.m_ships.begin(); it != shipManager.m_ships.end(); it++) {
		collectProjectile(it->first);
	}
}

void ProjectileManager::updateProjectiles(const BoundedFloatRect& worldBounds)
{
	for (std::list<Projectile>::iterator it = m_projectiles.begin(); it != m_projectiles.end(); it++)
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

bool ProjectileManager::detectCollision(const sf::FloatRect& gameObject, bool destroyProjectileInCollision)
{
	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); it++) {
		if (it->getGlobalBounds().intersects(gameObject)) {
			std::cout << "Collision Detected" << std::endl;
			if(destroyProjectileInCollision)
				m_projectiles.erase(it);
			return true;
		}
	}
	return false;
}

void ProjectileManager::detectCollision(ShipManager& shipManager, int& killCounter)
{
	auto it = shipManager.m_ships.begin();
	while (it != shipManager.m_ships.end()) {
		if (detectCollision(it->first.getGlobalBounds())) {
			it = shipManager.m_ships.erase(it);//increments the iterator
			std::cout << "Destroy Ship!" << std::endl;
			killCounter++;
		}
		else {
			it++;
		}
	}
}

void ProjectileManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); it++) {
		target.draw(*it);
	}
}

