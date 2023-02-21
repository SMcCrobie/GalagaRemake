#include "projectile.h"

Projectile::Projectile(const sf::Vector2f& size, const sf::Vector2f& velocity)
{
	setSize(size);
	m_velocity = velocity;
}

const sf::Vector2f& Projectile::getVelocity() const
{
	return m_velocity;
}

bool Projectile::setVelocity(const sf::Vector2f& velocity)
{
	if (velocity.y > PROJECTILE_START_SPEED)
		return false;
	m_velocity = velocity;
	return true;
}

void Projectile::move()
{
	sf::RectangleShape::move(m_velocity);
}
