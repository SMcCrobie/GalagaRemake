#pragma once
#include <list>
#include <SFML/Graphics/Drawable.hpp>

#include "GameObject.h"
#include "IManager.h"

class GameObjectManager
	: public sf::Drawable, public IManager
{
public:
	void createGameObject(const GameObject& obj);
	void createItem(const Item& item);
	[[nodiscard]] bool isEmpty() const;
	[[nodiscard]] int count() const;
	void resetManager() override;
	void update();
	void detectCollision(PlayerShip& playerShip);

private:
	std::list<GameObject> m_gameObjects;
	std::list<Item> m_items;
	ItemType detectCollisionWithItems(const PlayerShip& playerShip);
	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

