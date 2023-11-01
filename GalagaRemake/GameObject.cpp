#include "GameObject.h"

GameObject::GameObject()
{
	m_isThereSprite = false;
	m_isThereRect = false;
	m_isThereCircle = false;
}

void GameObject::setSprite(const sf::Sprite& sprite)
{
	m_sprite = sprite;
	m_isThereSprite = true;
}

void GameObject::setRect(const sf::RectangleShape& rect)
{
	m_rectangle = rect;
	m_isThereRect = true;
}

void GameObject::setCircle(const sf::CircleShape& circle)
{
	m_circle = circle;
	m_isThereCircle = true;
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

void GameObject::updateGameObject()
{
	moveObject();
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
