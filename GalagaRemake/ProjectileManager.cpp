
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

void ProjectileManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); it++) {
		target.draw(*it);
	}
}

