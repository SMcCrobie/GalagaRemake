#include <iostream>
#include "ProjectileManager.h"
#include "Collision.h"
#include "GameState.h"


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

void ProjectileManager::updateProjectiles()
{
	auto it = m_projectiles.begin();
	while (it != m_projectiles.end())
	{
		(*it)->updateProjectile();
		sf::FloatRect projectileBounds = (*it)->getGlobalBounds();
		if (GameState::world_bounds.intersects(projectileBounds))
			it++;
		else {
			it = m_projectiles.erase(it);
			if (it == m_projectiles.end())
				break;
		}
	}
}



//bool ProjectileManager::detectCollisionAndDestroyProjectile(const sf::FloatRect& gameObject)
//{
//	auto it = findProjectileInCollision(gameObject);
//	if (it == m_projectiles.end())
//		return false;
//	m_projectiles.erase(it);
//	return true;
//}
//
//bool ProjectileManager::detectCollisionAndDestroyProjectile(const sf::Sprite& sprite)
//{
//	auto it = findProjectileInCollision(sprite);
//	if (it == m_projectiles.end())
//		return false;
//	m_projectiles.erase(it);
//	return true;
//}
//
//bool ProjectileManager::detectCollisionAndDestroyProjectile(const CircleProjectile& shield)
//{
//	auto it = findProjectileInCollision(shield);
//	if (it == m_projectiles.end())
//		return false;
//	m_projectiles.erase(it);
//	return true;
//}

CollisionResult ProjectileManager::detectCollisionAndDestroyProjectile(
	const sf::FloatRect& gameObject)
{
	const auto it = findProjectileInCollision(gameObject);
	if (it == m_projectiles.end())
		return {};
	const auto vel = (*it)->getVelocity();
	m_projectiles.erase(it);
	return CollisionResult(vel* PROJECTILE_MASS);
}

CollisionResult ProjectileManager::detectCollisionAndDestroyProjectile(
	const sf::Sprite& sprite)
{
	const auto it = findProjectileInCollision(sprite);
	if (it == m_projectiles.end())
		return {};
	const auto vel = (*it)->getVelocity();
	m_projectiles.erase(it);
	return CollisionResult(vel * PROJECTILE_MASS);
}

CollisionResult ProjectileManager::detectCollisionAndDestroyProjectile(const CircleProjectile& shield)
{
	const auto it = findProjectileInCollision(shield);
	if (it == m_projectiles.end())
		return {};
	const auto vel = (*it)->getVelocity();
	m_projectiles.erase(it);
	return CollisionResult(vel * PROJECTILE_MASS);
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

std::list<std::shared_ptr<Projectile>>::iterator ProjectileManager::findProjectileInCollision(const sf::Sprite& sprite)
{
	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); it++) {
		//auto& explicitProjectile = (*it)->getPointCount() > 10 ? std::dynamic_pointer_cast<CircleProjectile>(*it) : std::dynamic_pointer_cast<RectangleProjectile>(*it);
		if((*it)->getPointCount() > 10)
		{
			if (Collision::pixelPerfectTest(sprite, *std::dynamic_pointer_cast<CircleProjectile>(*it))) {
				std::cout << "Collision Detected" << std::endl;
				return it;
			}
			
		}
		else
		{
			if (Collision::pixelPerfectTest(sprite, *std::dynamic_pointer_cast<RectangleProjectile>(*it))) {
				std::cout << "Collision Detected" << std::endl;
				return it;
			}
			
		}
	}
	return m_projectiles.end();
}

std::list<std::shared_ptr<Projectile>>::iterator ProjectileManager::findProjectileInCollision(
	const CircleProjectile& shield)
{
	//think I can redirect to the projectile and call from there, add detectCollision to projectile classes
	//safer then dynamically casting probably
	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); it++) {
		if ((*it)->getPointCount() > 10)
		{
			if (Collision::pixelPerfectTest(shield, *std::dynamic_pointer_cast<CircleProjectile>(*it))) {
				std::cout << "Collision Detected" << std::endl;
				return it;
			}

		}
		else
		{
			if (Collision::pixelPerfectTest(shield, *std::dynamic_pointer_cast<RectangleProjectile>(*it))) {
				std::cout << "Collision Detected" << std::endl;
				return it;
			}

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

void ProjectileManager::resetManager()
{
	m_projectiles.clear();
}

