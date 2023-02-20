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
	FireWeapon2,
	Rotate,

	InvalidShipControl
};
const float SINGLE_THRUST_DIRECTION_INCREMENT = .25f;
const float HORIZONTAL_DIRECTION_INCREMENT = .35f;
const float RESISTENCE_MULTIPLIER = .965f;
float const MAX_HORIZONTAL_SPEED = 14.0f;
const float MAX_VERTICAL_SPEED = 10.0f;
const float WORLD_BOUNDS_MARGIN = 10.0f;
const float WORLD_VIEW_MOVEMENT = -1.25f;


class Ship :
    public sf::Sprite
{
	friend class KeyboardController;
	friend class StateMachineController;
	friend class ProjectileManager;

public:
    Ship();
	void setTextureRectBasedOnShipState();
	void setProjectile(const Projectile& projectile);
	void setIsWorldBound(bool isWorldBound);
	void updateShipVelocity(BoundedFloatRect worldBounds);
	void move();
	void rotateIfTriggered();
	void rotate180();
	const std::map<ShipControl, bool>& getShipControlStateMappings();


private:
	virtual std::optional<Projectile> fireWeapon1IfFired();
	virtual std::optional<Projectile> fireWeapon2IfFired();

	bool m_isBackwards;
	bool m_isWorldBound;
	float m_horizontalDirectionIncrement;
	float m_verticalDirectionIncrement;
	sf::Vector2i m_shipAnimationFrame;
    sf::Vector2f m_velocity;
    sf::RectangleShape m_collisionBox;
	Projectile m_weapon1Projectile;
	Projectile m_weapon2Projectile;
    std::map<ShipControl, bool> m_shipControlsStateMappings;
};

