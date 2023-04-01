#include "GameObject.h"

GameObject::GameObject()
{
	m_collisonBox = sf::RectangleShape(sf::Vector2f(20.f, 20.f));
	m_collisonBox.setFillColor(sf::Color::Red);
}

GameObject::GameObject(const sf::RectangleShape& rectangleShape)
{
	m_collisonBox = rectangleShape;
	
}

void GameObject::setPosition(float x, float y)
{
	m_collisonBox.setPosition(x, y);
}

void GameObject::setPosition(const sf::Vector2f& position)
{
	m_collisonBox.setPosition(position);
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_collisonBox, states);
}
