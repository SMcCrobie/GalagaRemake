#include "projectile.h"

RectangleProjectile::RectangleProjectile(const sf::Vector2f& size, const sf::Vector2f& velocity)
{
	setSize(size);
	m_velocity = velocity;
}

const sf::Vector2f& RectangleProjectile::getVelocity() const
{
	return m_velocity;
}

bool RectangleProjectile::setVelocity(const sf::Vector2f& velocity)
{
	if (velocity.y > PROJECTILE_START_SPEED)
		return false;
	m_velocity = velocity;
	return true;
}

void RectangleProjectile::move()
{
	sf::RectangleShape::move(m_velocity);
}
