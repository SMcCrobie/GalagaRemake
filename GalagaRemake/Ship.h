#pragma once
#include <SFML/Graphics.hpp>
#include "BoundedFloatRect.h"
#include "Projectile.h"
#include <optional>


enum ShipControl
{
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
	friend class ShipManager;

public:
	
    Ship();
	virtual void updateShip(BoundedFloatRect worldBounds);
	virtual void setTextureRectBasedOnShipState();


	bool detectCollision(ProjectileManager& projectileManager);
	void setProjectile1(const RectangleProjectile& projectile);
	void setProjectile1(const CircleProjectile& projectile);
	void setProjectile2(const RectangleProjectile& projectile);
	void setProjectile2(const CircleProjectile& projectile);
	void setIsHorizontallyWorldBound(bool isHorizontallyWorldBound);
	void setVelocity(float x, float y);
	void updateRespawnTimer();
	void setStatic();
	void decrementShieldHealth();
	void setTextureRect(const sf::IntRect& rectangle);
	void applyVerticalResistance();
	void applyHorizontalResistance();
	void moveShip();
	virtual void rotate180();
	virtual void initRotation();
	virtual void respawnShip();
	virtual void respawnShip(int respawnTimer);
	bool isBackwards() const;
	bool isRespawning() const;
	bool hasShield() const;
	void setShield(const CircleProjectile& shield, int shieldHealth = 10);
	const CircleProjectile& getShield();
	const std::map<ShipControl, bool>& getShipControlStateMappings();
	void rotateIfTriggered();
	bool hasHealth() const;
	void decrementHealth();
	void setHealth(int healthTotal);
	const int& getHealth() const;
	void setShipColor(const sf::Color& color);
	void disableCurrentShipStates();
	void setWeaponRechargeTime(int gameCycles);
	void flipHorizontalMovementStates();
	void flipVerticalMovementStates();
	void setMovementIncrements(float horizontal, float up, float down);
	void setPointValue(int pointValue);
	int getPointValue() const;
	//void setStartHealth(int startHealth);//TODO fix this shit


protected:
	void updateShadingIfRespawning();
	void applyHorizontalVelocity();
	void applyVerticalVelocity();
	void applyStandardResistance();
	void testAndApplyVerticalWorldBounds(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
	void testAndApplyHorizontalWorldBounds(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);

	virtual void updateShipVelocity(BoundedFloatRect worldBounds);

	virtual std::optional<std::shared_ptr<Projectile>> fireWeapon1IfFired();
	virtual std::optional<std::shared_ptr<Projectile>> fireWeapon2IfFired();

	bool m_isBackwards;
	bool m_isStatic = false;
	int m_gameCyclesTillRespawned{};
	bool m_isVerticallyWorldBound;
	float m_horizontalDirectionIncrement;
	float m_moveUpIncrement;
	float m_moveDownIncrement;
	sf::Vector2i m_shipAnimationFrame;
    sf::Vector2f m_velocity;
    sf::RectangleShape m_collisionBox;
	CircleProjectile m_shield;
	int m_shieldHealth;
	int m_shipHealth;
	int m_shipHitTimer;
	sf::Color m_shipColor;
	std::shared_ptr<Projectile> m_weapon1Projectile;
	int m_weapon1ProjectileCounter;
	std::shared_ptr<Projectile> m_weapon2Projectile;
	int m_weapon2ProjectileCounter;
	int m_weaponRechargeTime;
    std::map<ShipControl, bool> m_shipControlsStateMappings;
	bool m_isTransitioning;
	float m_rotationIncrement;
	int m_startHealth;
	int m_pointValue;

private:
	void incrementRotation();
	void refreshColors();
	void setColor(const sf::Color& color);//makes higher level function private
	void applyStandardTexture();

};

