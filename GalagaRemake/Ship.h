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
	void moveShip();
	virtual void rotate180();
	void respawnShip();
	void respawnShip(int respawnTimer);
	bool isBackwards() const;
	bool isRespawning() const;
	bool hasShield() const;
	void setShield(const CircleProjectile& shield, const int shieldHealth = 10);
	const CircleProjectile& getShield();
	const std::map<ShipControl, bool>& getShipControlStateMappings();
	virtual void rotateIfTriggered();
	bool hasHealth() const;
	void decrementHealth();
	void setHealth(int healthTotal);
	const int& getHealth() const;
	void setShipColor(const sf::Color& color);

protected:
	void updateShadingIfRespawning();
	void applyBackwardsVelocity();//kinda not needed
	void applyStandardVelocity();
	void applyStandardTexture();
	void applyBackwardsTexture();
	//sf::IntRect MoveRight();
	virtual void updateShipVelocity(BoundedFloatRect worldBounds);

	virtual std::optional<std::shared_ptr<Projectile>> fireWeapon1IfFired();
	virtual std::optional<std::shared_ptr<Projectile>> fireWeapon2IfFired();

	bool m_isBackwards;
	bool m_isStatic = false;
	int m_gameCyclesTillRespawned{};
	bool m_isHorizontallyWorldBound;
	float m_horizontalDirectionIncrement;
	float m_verticalDirectionIncrement;
	sf::Vector2i m_shipAnimationFrame;
    sf::Vector2f m_velocity;
    sf::RectangleShape m_collisionBox;
	CircleProjectile m_shield;
	int m_shieldHealth;
	int m_shipHealth;
	int m_shipHitTimer;
	sf::Color m_shipColor;
	std::shared_ptr<Projectile> m_weapon1Projectile;
	std::shared_ptr<Projectile> m_weapon2Projectile;
    std::map<ShipControl, bool> m_shipControlsStateMappings;

private:
	void refreshColors();
	void setColor(const sf::Color& color);//makes higher level function private
};

