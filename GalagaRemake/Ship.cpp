#include "Ship.h"

#include "GameState.h"
#include "ProjectileManager.h"

Ship::Ship() : sf::Sprite()
{
	m_velocity = sf::Vector2f(0.0f, 0.0f);
	m_collisionBox = sf::RectangleShape();
	m_shipControlsStateMappings = std::map<ShipControl, bool>();
	m_shipAnimationFrame = sf::Vector2i(45, 48);
	RectangleProjectile temp(sf::Vector2f(3, 12));
	temp.setFillColor(sf::Color(5, 236, 241));
	m_weapon1Projectile = std::make_shared<RectangleProjectile>(temp);
	/*m_weapon1Projectile.setOutlineColor(sf::Color(255, 255, 255, 30));
	m_weapon1Projectile.setOutlineThickness(3.f);*/
	m_horizontalDirectionIncrement = HORIZONTAL_DIRECTION_INCREMENT;
	m_moveUpIncrement = SINGLE_THRUST_DIRECTION_INCREMENT;
	m_moveDownIncrement = SINGLE_THRUST_DIRECTION_INCREMENT / 4;
	m_isBackwards = false;
	m_isVerticallyWorldBound = true;
	m_shieldHealth = 0;
	m_shipHealth = 1;
	m_shipHitTimer = 0;
	m_shipColor = sf::Color::White;
	m_weapon1ProjectileCounter = 0;
	m_weapon2ProjectileCounter = 0;
	m_weaponRechargeTime = 0;
	
	for (int i = MoveUp; i < InvalidShipControl; i++) {
		m_shipControlsStateMappings[static_cast<ShipControl>(i)] = false;
	}
	
}


void Ship::refreshColors()
{
	m_shield.refreshColor();
	if (m_shipHitTimer < 0)
		return;
	if(m_shipHitTimer == 0)
		setColor(m_shipColor);
	m_shipHitTimer--;
}

void Ship::updateShip(BoundedFloatRect worldBounds)
{
	updateRespawnTimer();
	if (m_isStatic)
		return;
	rotateIfTriggered();
	updateShipVelocity(worldBounds);
	moveShip();
	refreshColors();
		
}


//currently requires a 3x3 sprite sheet, would be better to have it adapatable to sprite sheet if possible
void Ship::setTextureRectBasedOnShipState()
{
	if (m_gameCyclesTillRespawned == 100) {
		setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x, 0), m_shipAnimationFrame));
		return;
	}
	applyStandardTexture();

}

bool Ship::detectCollision(ProjectileManager& projectileManager)
{
	if (hasShield())
	{
		if (projectileManager.detectCollisionAndDestroyProjectile(m_shield)) {
			decrementShieldHealth();
			return true;
		}

	}
	else if (hasHealth())
	{
		if (projectileManager.detectCollisionAndDestroyProjectile(*this)) {
			decrementHealth();
			return true;
		}
	}
	return false;
}

void Ship::setProjectile1(const RectangleProjectile& projectile)
{
	m_weapon1Projectile = std::make_shared<RectangleProjectile>(projectile);
}

void Ship::setProjectile1(const CircleProjectile& projectile)
{
	m_weapon1Projectile = std::make_shared<CircleProjectile>(projectile);
}

void Ship::setProjectile2(const RectangleProjectile& projectile)
{
	m_weapon2Projectile = std::make_shared<RectangleProjectile>(projectile);
}

void Ship::setProjectile2(const CircleProjectile& projectile)
{
	m_weapon2Projectile = std::make_shared<CircleProjectile>(projectile);
}

void Ship::setIsHorizontallyWorldBound(const bool isHorizontallyWorldBound)
{
	m_isVerticallyWorldBound = isHorizontallyWorldBound;
}

void Ship::setVelocity(const float x, const float y)
{
	m_velocity = sf::Vector2f(x, y);
}

void Ship::updateRespawnTimer()
{
	if (m_gameCyclesTillRespawned <= 0)
		return;
	updateShadingIfRespawning();
	m_gameCyclesTillRespawned--;
}

void Ship::setStatic()
{
	m_isStatic = true;
}

void Ship::setShipColor(const sf::Color& color)
{
	setColor(color);
	m_shipColor = color;
}

void Ship::disableCurrentShipStates()
{
	for (auto& [shipControl, state] : m_shipControlsStateMappings)
	{
		state = false;
	}
}

void Ship::setWeaponRechargeTime(const int gameCycles)
{
	m_weaponRechargeTime = gameCycles;
}


void Ship::decrementHealth()
{
	if (m_shipHealth <= 0)
		return;
	m_shipHealth--;
	m_shipHitTimer += 3;

	setColor(sf::Color::Red);
}

//this is bad
void Ship::decrementShieldHealth()
{
	if(m_shieldHealth <= 0 )
	{
		m_shield.setFillColor(sf::Color::Transparent);
		m_shield.setOutlineColor(sf::Color::Transparent);
		return;
	}
	auto textRect = m_shield.getTextureRect();
	const int spriteFrameWidth = textRect.width;
	if (m_shieldHealth < 10 && textRect.left == spriteFrameWidth*2)
	{
		textRect.left += spriteFrameWidth;
		m_shield.setTextureRect(textRect);
	}
	else if (m_shieldHealth < 20 && textRect.left == spriteFrameWidth)
	{
		textRect.left += spriteFrameWidth;
		m_shield.setTextureRect(textRect);
	}
	else if (m_shieldHealth < 30 && textRect.left == 0)
	{
		textRect.left += spriteFrameWidth;
		m_shield.setTextureRect(textRect);
	}
	m_shieldHealth--;
	auto color = sf::Color::White;
	color.a = 100;
 	m_shield.setFillColor(color);
}

void Ship::setTextureRect(const sf::IntRect& rectangle)
{
	Sprite::setTextureRect(rectangle);
	m_shipAnimationFrame = sf::Vector2i(rectangle.width, rectangle.height);
}



void Ship::applyStandardResistance()
{
	if (m_velocity.y != 0)
		m_velocity.y = m_velocity.y * RESISTENCE_MULTIPLIER;

	if (m_velocity.x != 0)
		m_velocity.x = m_velocity.x * RESISTENCE_MULTIPLIER;
}

void Ship::updateShipVelocity(BoundedFloatRect worldBounds)
{
	applyStandardResistance();

	//apply thrust to shipVelocity, after State update, opposing Thrusts cancel
	applyVerticalVelocity();
	applyHorizontalVelocity();

	//WORLD BOUNDS, these are better but stil prob not best. Maybe derive sf::FloatRec and add bounds so you dont have to add the top and height or the left and width to get the right coordinate
	//ALSO, currently breaks if the ship is spawned outside the world bounds, should prob just manage that in ship spawning
	//Think each bound needs its own loop with a break if your already past it,
	//if the velocity is to get out leave it, if its to go against it, zero it out
	BoundedFloatRect shipBounds = getGlobalBounds();
	testAndApplyHorizontalWorldBounds(shipBounds, worldBounds);
	if (!m_isVerticallyWorldBound)
		return;
	testAndApplyVerticalWorldBounds(shipBounds, worldBounds);
}

void Ship::testAndApplyVerticalWorldBounds(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds)
{
	while (shipBounds.bottom + m_velocity.y > worldBounds.bottom - WORLD_BOUNDS_MARGIN
		|| shipBounds.top + m_velocity.y < worldBounds.top + WORLD_BOUNDS_MARGIN) {

		if (shipBounds.bottom > worldBounds.bottom - WORLD_BOUNDS_MARGIN) {
			m_velocity.y = 0;// (worldBounds.bottom - WORLD_BOUNDS_MARGIN) - shipBounds.bottom;
			break;
		}
		if (shipBounds.top < worldBounds.top + WORLD_BOUNDS_MARGIN) {
			m_velocity.y = 0;// shipBounds.top - (worldBounds.top + WORLD_BOUNDS_MARGIN);
			break;
		}
		m_velocity.y = m_velocity.y * .5f;

	}
}

void Ship::testAndApplyHorizontalWorldBounds(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds)
{
	while (shipBounds.right + m_velocity.x > worldBounds.right - WORLD_BOUNDS_MARGIN
		|| shipBounds.left + m_velocity.x < worldBounds.left + WORLD_BOUNDS_MARGIN) {



		if (abs(shipBounds.right - worldBounds.right - WORLD_BOUNDS_MARGIN) < .01f
			|| (abs(shipBounds.left - worldBounds.left + WORLD_BOUNDS_MARGIN) < .01f)) {
			m_velocity.x = 0;
			break;
		}
		m_velocity.x = m_velocity.x * .5f;



	}
}

void Ship::updateShadingIfRespawning()
{
	if (m_gameCyclesTillRespawned % 10 > 5) {
		setColor(sf::Color::Black);
	}
	else {
		setColor(sf::Color::White);
	}
}

void Ship::applyHorizontalVelocity()
{
	if (m_shipControlsStateMappings.at(MoveLeft) && m_velocity.x >= -MAX_HORIZONTAL_SPEED)
		m_velocity.x += -m_horizontalDirectionIncrement;
	if (m_shipControlsStateMappings.at(MoveRight) && m_velocity.x <= MAX_HORIZONTAL_SPEED)
		m_velocity.x += m_horizontalDirectionIncrement;
}

void Ship::applyVerticalVelocity()
{

	if (m_shipControlsStateMappings.at(MoveUp) && m_velocity.y >= -MAX_VERTICAL_SPEED)
		m_velocity.y += -m_moveUpIncrement;
	if (m_shipControlsStateMappings.at(MoveDown) && m_velocity.y <= MAX_VERTICAL_SPEED)
		m_velocity.y += m_moveDownIncrement;
}

void Ship::applyTexture()
{
	if (isBackwards())
		changeBackwardsTextureBasedOnMovementControl();
	else
		applyStandardTexture();
}

void Ship::changeBackwardsTextureBasedOnMovementControl()
{
	using namespace GameState;
	switch (movementControlSetting)
	{
	case full_window_orientation:
		applyBackwardsTexture2();
		break;
	case full_ship_orientation:
		applyStandardTexture();
		break;
	case window_and_ship_orientation:
		applyBackwardsTexture();
		break;
	}
}

void Ship::applyStandardTexture()
{
	if (m_shipControlsStateMappings.at(MoveLeft) && !m_shipControlsStateMappings.at(MoveRight)) {
		if (m_shipControlsStateMappings.at(MoveUp) && !m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x * 2, m_shipAnimationFrame.y), m_shipAnimationFrame));
		}
		else if (!m_shipControlsStateMappings.at(MoveUp) && m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(0, m_shipAnimationFrame.y), m_shipAnimationFrame));
		}
		else {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x, m_shipAnimationFrame.y), m_shipAnimationFrame));
		}
	}
	else if (!m_shipControlsStateMappings.at(MoveLeft) && m_shipControlsStateMappings.at(MoveRight)) {
		if (m_shipControlsStateMappings.at(MoveUp) && !m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x * 2, m_shipAnimationFrame.y * 2), m_shipAnimationFrame));
		}
		else if (!m_shipControlsStateMappings.at(MoveUp) && m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(0, m_shipAnimationFrame.y * 2), m_shipAnimationFrame));
		}
		else {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x, m_shipAnimationFrame.y * 2), m_shipAnimationFrame));
		}
	}
	else {
		if (m_shipControlsStateMappings.at(MoveUp) && !m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x * 2, 0), m_shipAnimationFrame));
		}
		else if (!m_shipControlsStateMappings.at(MoveUp) && m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(0, 0), m_shipAnimationFrame));
		}
		else {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x, 0), m_shipAnimationFrame));
		}
	}
}

void Ship::applyBackwardsTexture()
{
	if (!m_shipControlsStateMappings.at(MoveLeft) && m_shipControlsStateMappings.at(MoveRight)) {
		if (m_shipControlsStateMappings.at(MoveUp) && !m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x * 2, m_shipAnimationFrame.y), m_shipAnimationFrame));
		}
		else if (!m_shipControlsStateMappings.at(MoveUp) && m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(0, m_shipAnimationFrame.y), m_shipAnimationFrame));
		}
		else {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x, m_shipAnimationFrame.y), m_shipAnimationFrame));
		}
	}
	else if (m_shipControlsStateMappings.at(MoveLeft) && !m_shipControlsStateMappings.at(MoveRight)) {
		if (m_shipControlsStateMappings.at(MoveUp) && !m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x * 2, m_shipAnimationFrame.y * 2), m_shipAnimationFrame));
		}
		else if (!m_shipControlsStateMappings.at(MoveUp) && m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(0, m_shipAnimationFrame.y * 2), m_shipAnimationFrame));
		}
		else {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x, m_shipAnimationFrame.y * 2), m_shipAnimationFrame));
		}
	}
	else {
		if (m_shipControlsStateMappings.at(MoveUp) && !m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x * 2, 0), m_shipAnimationFrame));
		}
		else if (!m_shipControlsStateMappings.at(MoveUp) && m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(0, 0), m_shipAnimationFrame));
		}
		else {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x, 0), m_shipAnimationFrame));
		}
	}
}

void Ship::applyBackwardsTexture2()
{
	if (!m_shipControlsStateMappings.at(MoveLeft) && m_shipControlsStateMappings.at(MoveRight)) {
		if (!m_shipControlsStateMappings.at(MoveUp) && m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x * 2, m_shipAnimationFrame.y), m_shipAnimationFrame));
		}
		else if (m_shipControlsStateMappings.at(MoveUp) && !m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(0, m_shipAnimationFrame.y), m_shipAnimationFrame));
		}
		else {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x, m_shipAnimationFrame.y), m_shipAnimationFrame));
		}
	}
	else if (m_shipControlsStateMappings.at(MoveLeft) && !m_shipControlsStateMappings.at(MoveRight)) {
		if (!m_shipControlsStateMappings.at(MoveUp) && m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x * 2, m_shipAnimationFrame.y * 2), m_shipAnimationFrame));
		}
		else if (m_shipControlsStateMappings.at(MoveUp) && !m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(0, m_shipAnimationFrame.y * 2), m_shipAnimationFrame));
		}
		else {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x, m_shipAnimationFrame.y * 2), m_shipAnimationFrame));
		}
	}
	else {
		if (!m_shipControlsStateMappings.at(MoveUp) && m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x * 2, 0), m_shipAnimationFrame));
		}
		else if (m_shipControlsStateMappings.at(MoveUp) && !m_shipControlsStateMappings.at(MoveDown)) {
			setTextureRect(sf::IntRect(sf::Vector2i(0, 0), m_shipAnimationFrame));
		}
		else {
			setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x, 0), m_shipAnimationFrame));
		}
	}
}

void Ship::moveShip() 
{
	move(m_velocity);
	m_shield.move(m_velocity);
}

void Ship::rotate180()//need to call after setting projectile
{
	m_isBackwards = !m_isBackwards;
	m_horizontalDirectionIncrement = -m_horizontalDirectionIncrement;
	m_moveUpIncrement = -m_moveUpIncrement;
	
	
	m_weapon1Projectile->setVelocity(sf::Vector2f(0, -m_weapon1Projectile->getVelocity().y));
	rotate(180.f);//origin is now bottom right, but global bounds still correctly gives top and left
	const sf::FloatRect localBounds = getGlobalBounds(); 
	if (m_isBackwards) {
		move(localBounds.width, localBounds.height);
	}
	else {
		move(-localBounds.width, -localBounds.height);
	}

}

void Ship::respawnShip()
{
	m_shipHealth = 3;
	m_gameCyclesTillRespawned = 100;
}

void Ship::respawnShip(const int respawnTimer)
{
	m_shipHealth = 3;
	m_gameCyclesTillRespawned = respawnTimer;
}

bool Ship::isBackwards() const
{
	return m_isBackwards;
}

bool Ship::isRespawning() const
{
	return m_gameCyclesTillRespawned > 0;
}

bool Ship::hasShield() const
{
	return m_shieldHealth > 0;
}

bool Ship::hasHealth() const
{
	return m_shipHealth > 0;
}

void Ship::setHealth(int healthTotal)
{
	m_shipHealth = healthTotal;
}

const int& Ship::getHealth() const
{
	return m_shipHealth;
}


void Ship::setShield(const CircleProjectile& shield, const int shieldHealth)
{
	m_shield = shield;
	m_shieldHealth = shieldHealth;
	const auto shipPosition = getGlobalBounds();
	m_shield.setPosition(shipPosition.left + shipPosition.width/2, shipPosition.top + shipPosition.height/2);
}

const CircleProjectile& Ship::getShield()
{
	return m_shield;
}

const std::map<ShipControl, bool>& Ship::getShipControlStateMappings()
{
	return m_shipControlsStateMappings;
}

void Ship::rotateIfTriggered()
{
	if (!m_shipControlsStateMappings.at(Rotate))
		return;
	m_shipControlsStateMappings[Rotate] = false;
	rotate180();
}


//really solid function
std::optional<std::shared_ptr<Projectile>> Ship::fireWeapon1IfFired()
{
	if(m_weapon1ProjectileCounter > 0)
	{
		m_weapon1ProjectileCounter--;
		return {};
	}
	if(m_weaponRechargeTime > 0 )
	{
		m_weapon1ProjectileCounter = m_weaponRechargeTime;
	}
	if (!m_shipControlsStateMappings[FireWeapon1] || m_weapon1Projectile == nullptr)
		return {};
	const BoundedFloatRect currentShipPosition = getGlobalBounds();
	const float verticalStartPoint = m_isBackwards ? currentShipPosition.bottom - m_weapon1Projectile->getGlobalBounds().height : currentShipPosition.top;
	const float horizontalStartPoint = currentShipPosition.left + (currentShipPosition.width / 2) - (m_weapon1Projectile->getGlobalBounds().width / 2);
	auto yVel = abs(m_weapon1Projectile->getVelocity().y);
	yVel = m_isBackwards ? yVel : -yVel;

	m_shipControlsStateMappings[FireWeapon1] = false;
	m_weapon1Projectile->initStartPosition(horizontalStartPoint, verticalStartPoint, isBackwards());
	m_weapon1Projectile->setVelocity(sf::Vector2f(0, yVel));
	return m_weapon1Projectile->clone();

}

std::optional<std::shared_ptr<Projectile>> Ship::fireWeapon2IfFired()
{
	if (m_weapon2ProjectileCounter > 0)
	{
		m_weapon2ProjectileCounter--;
		return {};
	}
	if (m_weaponRechargeTime > 0)
	{
		m_weapon2ProjectileCounter = m_weaponRechargeTime;
	}

	if (!m_shipControlsStateMappings[FireWeapon2] || m_weapon2Projectile == nullptr)
		return {};
	const BoundedFloatRect currentShipPosition = getGlobalBounds();
	const float verticalStartPoint = m_isBackwards ? currentShipPosition.bottom - m_weapon2Projectile->getGlobalBounds().height : currentShipPosition.top;
	const float horizontalStartPoint = currentShipPosition.left + (currentShipPosition.width / 2) - (m_weapon2Projectile->getGlobalBounds().width / 2);
	auto yVel = abs(m_weapon2Projectile->getVelocity().y);
	yVel = m_isBackwards ? yVel : -yVel;


	m_shipControlsStateMappings[FireWeapon2] = false;
	m_weapon2Projectile->initStartPosition(horizontalStartPoint, verticalStartPoint, isBackwards());
	m_weapon2Projectile->setVelocity(sf::Vector2f(0, yVel));
	return m_weapon2Projectile->clone();
}

void Ship::setColor(const sf::Color& color)
{
	Sprite::setColor(color);
}



