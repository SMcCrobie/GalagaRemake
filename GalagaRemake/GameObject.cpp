#include "GameObject.h"

#include <iostream>

#include "Collision.h"


GameObject::GameObject(): m_oscillationTimer(0), m_oscillationThreshold(0), m_angularVelocity(0)
{
	m_isThereSprite = false;
	m_isThereRect = false;
	m_isThereCircle = false;
	m_isThereCircle = false;
}

//remove with collision box option
void GameObject::setSprite(const sf::Sprite& sprite, const bool withCollisionBox)
{
	m_sprite = sprite;
	m_isThereSprite = true;
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0f, m_sprite.getLocalBounds().height / 2.0f);
	m_localCenterOfMass = m_sprite.getOrigin();

	if(withCollisionBox)
	{
		setRect(sf::RectangleShape(sf::Vector2f(m_sprite.getLocalBounds().width, m_sprite.getLocalBounds().height)));
		m_rectangle.setPosition(m_sprite.getPosition());
	}
}

void GameObject::setRect(const sf::RectangleShape& rect)
{
	m_rectangle = rect;
	m_isThereRect = true;
	m_rectangle.setOrigin(m_rectangle.getLocalBounds().width / 2.0f, m_rectangle.getLocalBounds().height / 2.0f);
	m_localCenterOfMass = m_rectangle.getOrigin();
}

void GameObject::setCircle(const sf::CircleShape& circle)
{
	m_circle = circle;
	m_isThereCircle = true;
	m_circle.setOrigin(m_circle.getRadius(), m_circle.getRadius());
	m_localCenterOfMass = m_circle.getOrigin();
}

void GameObject::setRotation(const float rotation)
{
	m_angularVelocity = rotation;
}

void GameObject::setVelocity(const sf::Vector2f& velocity)
{
	m_velocity = velocity;
}

void GameObject::setVelocity(const float x, const float y)
{
	m_velocity.x = x;
	m_velocity.y = y;
}

void GameObject::setScale(float x, float y)
{
	if (m_isThereSprite)
	{
		m_sprite.setScale(x, y);
	}
	if (m_isThereCircle)
	{
		m_circle.setScale(x, y);
	}
	if (m_isThereRect)
	{
		m_rectangle.setScale(x, y);
	}
}

sf::Vector2f GameObject::getVelocity() const
{
	return m_velocity;
}

void GameObject::setOscillation(const sf::Vector2f& scalar, int framesTillSwitch)
{
	m_scalar = scalar;
	m_oscillationThreshold = framesTillSwitch;
	m_oscillationTimer = 0;

}

void GameObject::setPosition(const float x, const float y)
{
	if (m_isThereSprite)
	{
		m_sprite.setPosition(x, y);
	}
	if (m_isThereCircle)
	{
		m_circle.setPosition(x, y);
	}
	if (m_isThereRect)
	{
		m_rectangle.setPosition(x, y);
	}
}

sf::Vector2f GameObject::getPosition() const
{
	if (m_isThereSprite)
	{
		return m_sprite.getPosition();
	}
	if (m_isThereCircle)
	{
		return m_circle.getPosition();
	}
	if (m_isThereRect)
	{
		return m_rectangle.getPosition();
	}

	return {};
}

void GameObject::setPointValue(const int pointValue)
{
	m_pointValue = pointValue;
}


void GameObject::rotateObject()
{
	if (m_angularVelocity == 0.0f)
		return;
	if (m_isThereSprite)
	{
		m_sprite.rotate(m_angularVelocity);
	}
	if (m_isThereCircle)
	{
		m_circle.rotate(m_angularVelocity);
	}
	if (m_isThereRect)
	{
		m_rectangle.rotate(m_angularVelocity);
	}
	

}

void GameObject::oscillateObject()
{
	if (m_scalar.x == 0.0f && m_scalar.y == 0.0f)
		return;

	if (m_isThereSprite)
	{
		m_sprite.scale(m_scalar);
	}
	if (m_isThereCircle)
	{
		m_circle.scale(m_scalar);
	}
	if (m_isThereRect)
	{
		m_rectangle.scale(m_scalar);
	}

	m_oscillationTimer++;
	if (m_oscillationTimer > m_oscillationThreshold)
	{
		m_scalar.x = 1.0f / m_scalar.x;
		m_scalar.y = 1.0f / m_scalar.y;
		m_oscillationTimer = 0;
	}

}


void GameObject::move()
{
	if(m_velocity.x == 0.0f && m_velocity.y == 0.0f)
		return;
	if (m_isThereSprite)
	{
		m_sprite.move(m_velocity);
	}
	if (m_isThereCircle)
	{
		m_circle.move(m_velocity);
	}
	if (m_isThereRect)
	{
		m_rectangle.move(m_velocity);
	}
}


void GameObject::update()
{
	move();
	rotateObject();
	oscillateObject();
}

bool GameObject::detectCollision(const PlayerShip& playerShip) const
{
	//refactor this to be like other detect collision
	if (m_isThereSprite && m_isThereRect)
	{
		if (Collision::pixelPerfectTest(playerShip, m_rectangle))
		{
			std::cout << "Collision Detected" << std::endl;
			return true;
		}
	}
	return false;
}

int GameObject::getPointValue() const
{
	return m_pointValue;
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(m_isThereSprite)
	{
		target.draw(m_sprite, states);
		return;
	}
	if(m_isThereCircle)
	{
		target.draw(m_circle, states);
		return;
	}
	if (m_isThereRect) {
		target.draw(m_rectangle, states);
	}
}




//Item
//--------------------------------------------------------------------------

ItemType Item::getItemType() const
{
	return m_itemType;
}

void Item::setItemType(ItemType type)
{
	m_itemType = type;
}

void Collidable::setHealth(const int health)
{
	m_health = health;
}

int Collidable::getHealth() const
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

void Collidable::decrementHealth()
{
	if(m_health > 0)
	{
		m_objectHitTimer = 2;
		m_health--;
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
	const auto currentMomentum = getMomentum();
	const auto currentAngularVelocity = getAngularVelocity();

	applyMomentum(collidable.getMomentum(), pointOfImpact);
	applyAngularVelocity(collidable.getAngularVelocity(), pointOfImpact, collidable.getMomentum());

	collidable.applyMomentum(currentMomentum, pointOfImpact);
	collidable.applyAngularVelocity(currentAngularVelocity, pointOfImpact, currentMomentum);
}

void Collidable::applyPhysicsFromProjectile(const CollisionResult collisionResult)
{
	applyMomentum(collisionResult.momentum);
	applyAngularVelocity(collisionResult.rotationalVelocity, collisionResult.pointOfImpact, collisionResult.momentum * 4.5f);
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
		if(pointOfImpact.has_value())
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
		if (m_rectangle.getGlobalBounds().intersects(rect.getGlobalBounds(), intersection ))
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
	if (m_objectHitTimer < 0 )
		return;
	if (m_objectHitTimer == 0)
		setColor(sf::Color::White);
	m_objectHitTimer--;
}
