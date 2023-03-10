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
//	Ship(const Ship& ship);Caused more issues for some reason
	virtual void setTextureRectBasedOnShipState();
	void setProjectile(const RectangleProjectile& projectile);
	void setIsHorizontallyWorldBound(bool IsHorizontallyWorldBound);
	void setVelocity(float x, float y);
	void updateShip(BoundedFloatRect worldBounds);
	void updateRespawnTimer();
	void setStatic();
	void moveShip();
	//void rotateIfTriggered();
	virtual void rotate180();
	void respawnShip();
	bool isBackwards();
	bool isRespawning();
	const std::map<ShipControl, bool>& getShipControlStateMappings();


protected:
	void updateShadingIfRespawning();
	void applyBackwardsVelocity();//kinda not needed
	void applyStandardVelocity();
	void applyStandardTexture();
	void applyBackwardsTexture();
	//sf::IntRect MoveRight();
	virtual void updateShipVelocity(BoundedFloatRect worldBounds);

	virtual std::optional<RectangleProjectile> fireWeapon1IfFired();
	virtual std::optional<RectangleProjectile> fireWeapon2IfFired();

	bool m_isBackwards;
	bool m_isStatic = false;
	int m_gameCyclesTillRespawned;
	bool m_isHorizontallyWorldBound;
	float m_horizontalDirectionIncrement;
	float m_verticalDirectionIncrement;
	sf::Vector2i m_shipAnimationFrame;
    sf::Vector2f m_velocity;
    sf::RectangleShape m_collisionBox;
	RectangleProjectile m_weapon1Projectile;
	RectangleProjectile m_weapon2Projectile;
    std::map<ShipControl, bool> m_shipControlsStateMappings;
};

