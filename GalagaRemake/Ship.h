#pragma once
#include <SFML/Graphics.hpp>
#include "BoundedFloatRect.h"
enum ShipControl {
	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight,
	FireWeapon1,
	
	FireWeapon2
};
const float SINGLE_THRUST_DIRECTION_INCREMENT = .25;
const float HORIZONTAL_DIRECTION_INCREMENT = .35;
const float RESISTENCE_MULTIPLIER = .96;
float const MAX_HORIZONTAL_SPEED = 10;
const float MAX_VERTICAL_SPEED = 8;
const float WORLD_BOUNDS_MARGIN = 10;
const float WORLD_VIEW_MOVEMENT = -1.25f;


class Ship :
    public sf::Sprite
{
	friend class Controller;

public:
    Ship();
	void setTextureRectBasedOnShipState();
	void updateShipVelocity(BoundedFloatRect worldBounds);
	void move();

	//Hacky, should be handled some other way
	bool isFire1 = false;


private:
	sf::Vector2i m_shipAnimationFrame;
    sf::Vector2f m_velocity;
    sf::RectangleShape m_collisionBox;
    std::map<ShipControl, bool> m_shipControlsStateMappings;
};

