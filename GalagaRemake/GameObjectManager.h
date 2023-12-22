#pragma once
#include <list>
#include <SFML/Graphics/Drawable.hpp>

#include "Collidable.h"
#include "GameObject.h"
#include "IManager.h"
#include "Item.h"

class GameObjectManager
	: public sf::Drawable, public IManager
{
public:
	void createGameObject(const GameObject& obj);
	void createItem(const Item& item);
	void createCollidable(Collidable& collidable);
	[[nodiscard]] bool isItemsEmpty() const;
	[[nodiscard]] size_t itemsCount() const;
	[[nodiscard]] bool isCollidablesEmpty() const;
	[[nodiscard]] size_t collidablesCount() const;
	void resetManager() override;
	void update();
	void detectItemCollision(PlayerShip& playerShip);

private:
	std::list<GameObject> m_gameObjects;
	std::list<Collidable> m_collidables;//might be better as list
	std::list<Item> m_items;

	void updateCollidables();
	void updateItems();
	void updateGameObjects();
	static bool checkIfOutOfBounds(const Collidable& collidable);
	std::optional<ItemType> detectCollisionWithItems(const PlayerShip& playerShip);
	static void addItemPointValueToScore(const Item& item, const PlayerShip& playerShip);
	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

