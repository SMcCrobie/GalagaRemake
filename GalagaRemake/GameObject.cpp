#include "GameObject.h"
#include <typeinfo>


using namespace std;

GameObject::GameObject()
	: m_collisonBox(make_unique<sf::RectangleShape>(sf::Vector2f(20.f, 20.f)))
{
	m_collisonBox->setFillColor(sf::Color::Red);
}

GameObject::GameObject(sf::RectangleShape collisonBox)
	: m_collisonBox(make_unique<sf::RectangleShape>(collisonBox))
{
}

GameObject::GameObject(sf::Sprite sprite)
	: m_sprite(make_unique<sf::Sprite>(sprite))
{
}

GameObject::GameObject(sf::Sprite sprite, sf::RectangleShape collisonBox)
	: m_sprite(make_unique<sf::Sprite>(sprite)),
	m_collisonBox(make_unique<sf::RectangleShape>(collisonBox))
{
}

GameObject::GameObject(const GameObject& gameObject)
	: m_sprite(gameObject.m_sprite ? make_unique<sf::Sprite>(*gameObject.m_sprite) : nullptr)
{
	if (gameObject.m_collisonBox == nullptr)
		return;
	string classType = typeid(*gameObject.m_collisonBox).name();
	if (classType == "class sf::RectangleShape") {
		m_collisonBox = make_unique<sf::RectangleShape>(sf::RectangleShape(*dynamic_pointer_cast<sf::RectangleShape>(gameObject.m_collisonBox)));
	}

}


void GameObject::setPosition(float x, float y)
{
	m_collisonBox->setPosition(x, y);
}

void GameObject::setPosition(const sf::Vector2f& position)
{
	m_collisonBox->setPosition(position);
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_collisonBox == nullptr && m_sprite == nullptr)
		return;
	target.draw(*m_collisonBox, states);
}
