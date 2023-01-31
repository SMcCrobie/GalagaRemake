#include "projectile.h"

Projectile::Projectile(const sf::Vector2f& size, const sf::Vector2f& velocity)
{
	this->setSize(size);
	this->velocity = velocity;
}

const sf::Vector2f& Projectile::getVelocity() const
{
	return this->velocity;
}

bool Projectile::setVelocity(const sf::Vector2f& velocity)
{
	if (velocity.y > PROJECTILE_START_SPEED)
		return false;
	this->velocity = velocity;
	return true;
}

void Projectile::moveProjectile()
{
	move(this->velocity);
	//this->velocity = this->velocity * PROJECTILE_RESISTENCE_MULTIPLIER;
}
