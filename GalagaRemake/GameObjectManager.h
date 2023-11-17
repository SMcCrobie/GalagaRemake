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
	void createCollidable(const Collidable& collidable);
	/*[[nodiscard]] bool isEmpty() const;
	[[nodiscard]] int count() const;*/
	[[nodiscard]] bool isItemsEmpty() const;
	[[nodiscard]] int itemsCount() const;
	[[nodiscard]] bool isCollidablesEmpty() const;
	[[nodiscard]] int collidablesCount() const;
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
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

