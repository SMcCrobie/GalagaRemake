#include "Collidable.h"

#include "Collision.h"


float Collidable::vectorMagnitude(const sf::Vector2f& vector) const
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}


void Collidable::setHealth(const float health)
{
	m_health = health;
}

float Collidable::getHealth() const
{
	return m_health;
}

sf::Vector2f Collidable::getMomentum() const
{
	return m_mass * m_velocity;
}

void Collidable::setColor(const sf::Color& color)
{
	m_objectHitTimer += 3;
	if (m_isThereSprite)
	{
		m_sprite.setColor(color);
		return;
	}
	if (m_isThereCircle)
	{
		m_circle.setFillColor(color);
		return;
	}
	if (m_isThereRect) {
		m_rectangle.setFillColor(color);
	}
}

void Collidable::decrementHealth(const sf::Vector2f& changeInMomentum)
{
	constexpr auto impactScalar = .15f;
	if (m_health > 0.f)
	{
		m_objectHitTimer = 2;
		m_health -= vectorMagnitude(changeInMomentum) * impactScalar;
		setColor(sf::Color(218, 160, 109));
	}
}

void Collidable::explode()
{
	//TODO add animation here
}

sf::Transform Collidable::getTransform() const
{
	if (m_isThereSprite)
	{
		return m_sprite.getTransform();
	}
	if (m_isThereCircle)
	{
		return m_circle.getTransform();
	}
	if (m_isThereRect) {
		return m_rectangle.getTransform();
	}
}

constexpr float coefficient_of_restitution = 0.65f;

void Collidable::applyMomentum(const sf::Vector2f momentum)
{
	//can be some value 0 to 1 to simulate kinetic energy lost in the collision to other factors
	m_velocity = (getMomentum() + momentum) / m_mass * coefficient_of_restitution;
}

void Collidable::applyMomentum(sf::Vector2f momentum, const sf::Vector2f impactLocation)
{
	//can be some value 0 to 1 to simulate kinetic energy lost in the collision to other factors

	const sf::Vector2f centerOfMass = getTransform().transformPoint(m_localCenterOfMass);

	// Calculate the vector from the impact location to the center of mass
	const sf::Vector2f impactToCenter = centerOfMass - impactLocation;

	// Calculate the cross product of the impact-to-center vector and the momentum vector
	const float crossProduct = impactToCenter.x * momentum.y - impactToCenter.y * momentum.x;

	if (impactToCenter.x * momentum.x < 0)//less than zero is opposing
	{
		momentum.x = momentum.x * -0.6f;
	}
	if (impactToCenter.y * momentum.y < 0)
	{
		momentum.y = momentum.y * -0.6f;
	}

	m_velocity = ((getMomentum() + momentum) / m_mass + sf::Vector2f(crossProduct * impactToCenter.y, -crossProduct * impactToCenter.x) / (m_mass * impactToCenter.x * impactToCenter.x + m_mass * impactToCenter.y * impactToCenter.y)) * coefficient_of_restitution;
}


void Collidable::applyAngularVelocity(const float angularVelocity, sf::Vector2f impactLocation, sf::Vector2f momentum)
{
	const float relativeRotation = m_angularVelocity - angularVelocity;
	constexpr float spinnynessFactor = 2.f;

	const sf::Vector2f centerOfMass = getTransform().transformPoint(m_localCenterOfMass);

	// Calculate the vector from the impact location to the center of mass
	const sf::Vector2f impactToCenter = centerOfMass - impactLocation;

	// Calculate the cross product of the impact-to-center vector and the momentum vector
	const float crossProduct = impactToCenter.x * momentum.y - impactToCenter.y * momentum.x;

	// Calculate rotation matrix
	const float cosTheta = cos(relativeRotation * 3.14159f / 180.0f); // Convert degrees to radians
	const float sinTheta = sin(relativeRotation * 3.14159f / 180.0f);

	// Apply rotation matrix to impact vector
	const float rotatedImpactX = cosTheta * impactToCenter.x - sinTheta * impactToCenter.y;
	const float rotatedImpactY = sinTheta * impactToCenter.x + cosTheta * impactToCenter.y;

	// Calculate angular velocity change using the rotated impact vector
	const float angularVelocityChange = crossProduct / (m_mass * rotatedImpactX * rotatedImpactX + m_mass * rotatedImpactY * rotatedImpactY);

	// Update rotation
	m_angularVelocity -= angularVelocityChange * spinnynessFactor;
}

void Collidable::applyPhysicsToEachOther(Collidable& collidable, const sf::Vector2f pointOfImpact)
{
	const auto initialMomentum = getMomentum();
	const auto initialAngularVelocity = getAngularVelocity();
	const auto theirInitialMomentum = collidable.getMomentum();

	applyMomentum(collidable.getMomentum(), pointOfImpact);
	applyAngularVelocity(collidable.getAngularVelocity(), pointOfImpact, collidable.getMomentum());
	decrementHealth(getMomentum() - initialMomentum);

	collidable.applyMomentum(initialMomentum, pointOfImpact);
	collidable.applyAngularVelocity(initialAngularVelocity, pointOfImpact, initialMomentum);
	collidable.decrementHealth(collidable.getMomentum() - theirInitialMomentum);
}

void Collidable::applyPhysicsFromProjectile(const CollisionResult collisionResult)
{
	const auto initialMomentum = getMomentum();
	applyMomentum(collisionResult.momentum);
	applyAngularVelocity(collisionResult.rotationalVelocity, collisionResult.pointOfImpact, collisionResult.momentum * 4.5f);
	decrementHealth(getMomentum() - initialMomentum);
}



void Collidable::update()
{
	GameObject::update();
	updateObjectHitTimer();
}

std::optional<CollisionResult> Collidable::detectProjectileCollision()
{
	extern ProjectileManager playerProjectileManager;
	if (m_isThereCircle)
	{
		//TODO switch to circle shapes, everywhere
		return std::nullopt;//playerProjectileManager.detectCollisionAndDestroyProjectile(m_circle);
	}
	if (m_isThereRect) {
		const auto collisionResult = playerProjectileManager.detectCollisionAndDestroyProjectile(m_rectangle.getGlobalBounds());
		if (collisionResult.has_value())
			return collisionResult;
		return std::nullopt;
	}
	if (m_isThereSprite)
	{
		const auto collisionResult = playerProjectileManager.detectCollisionAndDestroyProjectile(m_sprite);
		if (collisionResult.has_value())
			return collisionResult;
		return std::nullopt;
	}
	return std::nullopt;

}

void Collidable::setMass(const float mass)
{
	m_mass = mass;
}

float Collidable::getAngularVelocity() const
{
	return m_angularVelocity;
}

std::optional<sf::Vector2f> Collidable::detectCollision(const Collidable& collidable) const
{
	if (m_isThereCircle)
	{
		//TODO switch to circle shapes, everywhere
		return std::nullopt;//playerProjectileManager.detectCollisionAndDestroyProjectile(m_circle);
	}
	if (m_isThereRect) {
		const auto pointOfImpact = collidable.detectCollision(m_rectangle);
		if (pointOfImpact.has_value())
			return pointOfImpact;
		return std::nullopt;
	}
	if (m_isThereSprite)
	{
		const auto pointOfImpact = collidable.detectCollision(m_sprite);
		if (pointOfImpact.has_value())
			return pointOfImpact;
		return std::nullopt;
	}
	return std::nullopt;
}

std::optional<sf::Vector2f> Collidable::detectCollision(const sf::RectangleShape& rect) const
{
	if (m_isThereCircle)
	{
		//todo
		return std::nullopt;
	}
	if (m_isThereRect) {
		sf::FloatRect intersection;
		if (m_rectangle.getGlobalBounds().intersects(rect.getGlobalBounds(), intersection))
			return sf::Vector2f(intersection.left, intersection.top);
		return std::nullopt;
	}

	if (m_isThereSprite)
	{
		auto pointOfImpact = Collision::pixelPerfectTest(m_sprite, rect);
		if (pointOfImpact.has_value())
			return pointOfImpact.value();
		return std::nullopt;
	}
	return std::nullopt;
}

std::optional<sf::Vector2f> Collidable::detectCollision(const sf::Sprite& sprite) const
{
	if (m_isThereCircle)
		return std::nullopt;

	if (m_isThereRect)
		return std::nullopt;

	if (m_isThereSprite)
		return Collision::pixelPerfectTest(m_sprite, sprite);
	return std::nullopt;
}

void Collidable::updateObjectHitTimer()
{
	if (m_objectHitTimer < 0)
		return;
	if (m_objectHitTimer == 0)
		setColor(sf::Color::White);
	m_objectHitTimer--;
}
