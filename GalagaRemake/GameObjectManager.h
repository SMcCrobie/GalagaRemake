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
	[[nodiscard]] bool isEmpty() const;
	[[nodiscard]] int count() const;
	void resetManager() override;
	void updateObjects();

private:
	std::list<GameObject> m_gameObjects;
	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

