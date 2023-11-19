#include "GameObject.h"

#include <iostream>

#include "Collision.h"

GameObject::GameObject(): m_oscillationTimer(0), m_oscillationThreshold(0), m_rotation(0)
{
	m_isThereSprite = false;
	m_isThereRect = false;
	m_isThereCircle = false;
}


//remove with collision box option
void GameObject::setSprite(const sf::Sprite& sprite, const bool withCollisionBox)
{
	m_sprite = sprite;
	m_isThereSprite = true;
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0f, m_sprite.getLocalBounds().height / 2.0f);

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
}

void GameObject::setCircle(const sf::CircleShape& circle)
{
	m_circle = circle;
	m_isThereCircle = true;
	m_circle.setOrigin(m_circle.getRadius(), m_circle.getRadius());
}

void GameObject::setRotation(const float rotation)
{
	m_rotation = rotation;
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
	if (m_rotation == 0.0f)
		return;
	if (m_isThereSprite)
	{
		m_sprite.rotate(m_rotation);
	}
	if (m_isThereCircle)
	{
		m_circle.rotate(m_rotation);
	}
	if (m_isThereRect)
	{
		m_rectangle.rotate(m_rotation);
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


void GameObject::moveObject()
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
	moveObject();
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
		m_objectHitTimer += 1;
		m_health--;
		setColor(sf::Color(218, 160, 109));
	}
}

void Collidable::explode()
{
	//TODO add animation here
}

void Collidable::applyMomentum(const sf::Vector2f momentum)
{
	m_velocity = (m_mass * m_velocity + momentum) / m_mass;
}

void Collidable::update()
{
	GameObject::update();
	updateObjectHitTimer();
}

bool Collidable::detectCollision()
{
	extern ProjectileManager playerProjectileManager;
	if (m_isThereCircle)
	{
		//TODO switch to circle shapes, everywhere
		return false;//playerProjectileManager.detectCollisionAndDestroyProjectile(m_circle);
	}
    if (m_isThereRect) {
		const auto collisionResult = playerProjectileManager.detectCollisionAndDestroyProjectile(m_rectangle.getGlobalBounds());
		if (!collisionResult.impact)
			return false;
		applyMomentum(collisionResult.impact.value());
		return true;
	}
	if (m_isThereSprite)
	{
		const auto collisionResult = playerProjectileManager.detectCollisionAndDestroyProjectile(m_sprite);
		if (!collisionResult.impact)
			return false;
		applyMomentum(collisionResult.impact.value());
		return true;
	}
	return false;

}

void Collidable::setMass(float mass)
{
	m_mass = mass;
}

void Collidable::updateObjectHitTimer()
{
	if (m_objectHitTimer < 0 )
		return;
	if (m_objectHitTimer == 0)
		setColor(sf::Color::White);
	m_objectHitTimer--;
}
