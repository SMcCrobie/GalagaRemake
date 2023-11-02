#include "GameObject.h"

#include <iostream>

#include "Collision.h"

GameObject::GameObject()
{
	m_isThereSprite = false;
	m_isThereRect = false;
	m_isThereCircle = false;
}

void GameObject::setSprite(const sf::Sprite& sprite, bool withCollisionBox)
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

void GameObject::setOscillation(const sf::Vector2f& scalar, int framesTillSwitch)
{
	m_scalar = scalar;
	m_oscillationThreshold = framesTillSwitch;
	m_oscillationTimer = 0;

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
	//TODO make other types of collisions
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

ItemType GameObject::getItemType() const
{
	return m_itemType;
}

void GameObject::setItemType(ItemType type)
{
	m_itemType = type;
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
