#pragma once
#include <SFML/Graphics.hpp>
#include "BoundedFloatRect.h"
#include "Projectile.h"
#include <optional>


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
const float RESISTENCE_MULTIPLIER = .965;
float const MAX_HORIZONTAL_SPEED = 14;
const float MAX_VERTICAL_SPEED = 10;
const float WORLD_BOUNDS_MARGIN = 10;
const float WORLD_VIEW_MOVEMENT = -1.25f;


class Ship :
    public sf::Sprite
{
	friend class Controller;
	friend class ProjectileManager;

public:
    Ship();
	void setTextureRectBasedOnShipState();
	void updateShipVelocity(BoundedFloatRect worldBounds);
	void move();
	const std::map<ShipControl, bool>& getShipControlStateMappings();


private:
	virtual std::optional<Projectile> fireWeapon1IfFired();
	virtual std::optional<Projectile> fireWeapon2IfFired();

	sf::Vector2i m_shipAnimationFrame;
    sf::Vector2f m_velocity;
    sf::RectangleShape m_collisionBox;
	Projectile m_weapon1Projectile;
	Projectile m_weapon2Projectile;
    std::map<ShipControl, bool> m_shipControlsStateMappings;
};

