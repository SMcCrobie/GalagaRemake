#include "Ship.h"
#include "ProjectileManager.h"

Ship::Ship() : sf::Sprite()
{
	m_velocity = sf::Vector2f(0.0f, 0.0f);
	m_collisionBox = sf::RectangleShape();
	m_shipControlsStateMappings = std::map<ShipControl, bool>();
	m_shipAnimationFrame = sf::Vector2i(45, 48);
	m_weapon1Projectile = std::make_shared<RectangleProjectile>( RectangleProjectile(sf::Vector2f(3, 12)));
	m_weapon1Projectile->setFillColor(sf::Color(5, 236, 241));
	/*m_weapon1Projectile.setOutlineColor(sf::Color(255, 255, 255, 30));
	m_weapon1Projectile.setOutlineThickness(3.f);*/
	m_horizontalDirectionIncrement = HORIZONTAL_DIRECTION_INCREMENT;
	m_verticalDirectionIncrement = SINGLE_THRUST_DIRECTION_INCREMENT;
	m_isBackwards = false;
	m_isHorizontallyWorldBound = true;
	m_shieldHealth = 0;
	m_shipHealth = 1;
	m_shipHitTimer = 0;
	m_shipColor = sf::Color::White;
	
	for (int i = MoveUp; i < InvalidShipControl; i++) {
		m_shipControlsStateMappings[static_cast<ShipControl>(i)] = false;
	}
	
}


void Ship::updateShip(BoundedFloatRect worldBounds)
{
	updateRespawnTimer();
	if (m_isStatic)
		return;
	updateShipVelocity(worldBounds);
	moveShip();
	m_shield.refreshColor();
	if (m_shipHitTimer < 0)
		return;
	if(m_shipHitTimer == 0)
		setColor(m_shipColor);
	m_shipHitTimer--;
		
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
	auto gbounds = hasShield() ? m_shield.getGlobalBounds() : getGlobalBounds();
	if (projectileManager.detectCollisionAndDestroyProjectile(gbounds)) {
		if (hasShield())
		{
			decrementShieldHealth();

		}
		else if (hasHealth())
		{
			decrementHealth();
		}
		return true;
	}

	return false;
}

void Ship::setProjectile(const std::shared_ptr<Projectile> projectile)
{
	m_weapon1Projectile = projectile;
}

void Ship::setIsHorizontallyWorldBound(const bool isHorizontallyWorldBound)
{
	m_isHorizontallyWorldBound = isHorizontallyWorldBound;
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
	if (m_shieldHealth < 25 && textRect.left == spriteFrameWidth*2)
	{
		textRect.left += spriteFrameWidth;
		m_shield.setTextureRect(textRect);
	}
	else if (m_shieldHealth < 50 && textRect.left == spriteFrameWidth)
	{
		textRect.left += spriteFrameWidth;
		m_shield.setTextureRect(textRect);
	}
	else if (m_shieldHealth < 75 && textRect.left == 0)
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

void Ship::updateShipVelocity(BoundedFloatRect worldBounds)
{

	BoundedFloatRect shipBounds = getGlobalBounds();
	//apply resistence
	if (m_velocity.y != 0)
		m_velocity.y = m_velocity.y * RESISTENCE_MULTIPLIER;

	if (m_velocity.x != 0)
		m_velocity.x = m_velocity.x * RESISTENCE_MULTIPLIER;

	//apply thrust to shipVelocity, after State update, opposing Thrusts cancel
	applyStandardVelocity();


	//WORLD BOUNDS, these are better but stil prob not best. Maybe derive sf::FloatRec and add bounds so you dont have to add the top and height or the left and width to get the right coordinate
	while (shipBounds.right + m_velocity.x > worldBounds.right - WORLD_BOUNDS_MARGIN
		|| shipBounds.left + m_velocity.x < worldBounds.left + WORLD_BOUNDS_MARGIN) {

		if (abs(shipBounds.right - worldBounds.right + WORLD_BOUNDS_MARGIN) < .01f
			|| abs(shipBounds.left - worldBounds.left - WORLD_BOUNDS_MARGIN) < .01f) {
			m_velocity.x = 0;
			break;
		}
		m_velocity.x = m_velocity.x * .5f;

	}

	if (!m_isHorizontallyWorldBound)
		return;

	while (shipBounds.bottom + m_velocity.y > worldBounds.bottom - WORLD_BOUNDS_MARGIN
		|| shipBounds.top + m_velocity.y < worldBounds.top + WORLD_BOUNDS_MARGIN) {

		if (abs(shipBounds.top - worldBounds.top - WORLD_BOUNDS_MARGIN) < .01f
			|| (abs(shipBounds.bottom - worldBounds.bottom + WORLD_BOUNDS_MARGIN) < .01f)) {
			m_velocity.x = 0;
			break;
		}
		m_velocity.y = m_velocity.y * .5f;

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

void Ship::applyBackwardsVelocity()
{
	if (m_shipControlsStateMappings.at(MoveUp) && m_velocity.y >= -MAX_VERTICAL_SPEED)
		m_velocity.y += -m_verticalDirectionIncrement / 3.f;
	if (m_shipControlsStateMappings.at(MoveDown) && m_velocity.y <= MAX_VERTICAL_SPEED)
		m_velocity.y += m_verticalDirectionIncrement;

	if (m_shipControlsStateMappings.at(MoveLeft) && m_velocity.x >= -MAX_HORIZONTAL_SPEED)
		m_velocity.x += -m_horizontalDirectionIncrement;
	if (m_shipControlsStateMappings.at(MoveRight) && m_velocity.x <= MAX_HORIZONTAL_SPEED)
		m_velocity.x += m_horizontalDirectionIncrement;
}

void Ship::applyStandardVelocity()
{
	if (m_shipControlsStateMappings.at(MoveUp) && m_velocity.y >= -MAX_VERTICAL_SPEED)
		m_velocity.y += -m_verticalDirectionIncrement;
	if (m_shipControlsStateMappings.at(MoveDown) && m_velocity.y <= MAX_VERTICAL_SPEED)
		m_velocity.y += m_verticalDirectionIncrement / 3.f;

	if (m_shipControlsStateMappings.at(MoveLeft) && m_velocity.x >= -MAX_HORIZONTAL_SPEED)
		m_velocity.x += -m_horizontalDirectionIncrement;
	if (m_shipControlsStateMappings.at(MoveRight) && m_velocity.x <= MAX_HORIZONTAL_SPEED)
		m_velocity.x += m_horizontalDirectionIncrement;
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
	m_verticalDirectionIncrement = -m_verticalDirectionIncrement;
	
	
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

std::optional<std::shared_ptr<Projectile>> Ship::fireWeapon1IfFired()
{
	if (!m_shipControlsStateMappings[FireWeapon1])
		return {};
	const BoundedFloatRect currentShipPosition = getGlobalBounds();
	const float verticalStartPoint = m_isBackwards ? currentShipPosition.bottom - m_weapon1Projectile->getGlobalBounds().height : currentShipPosition.top;
	const float horizontalStartPoint = currentShipPosition.left + (currentShipPosition.width / 2) - (m_weapon1Projectile->getGlobalBounds().width / 2);
	auto yVel = abs(m_weapon1Projectile->getVelocity().y);
	yVel = m_isBackwards ? yVel : -yVel;

	m_shipControlsStateMappings[FireWeapon1] = false;
	m_weapon1Projectile->initStartPosition(horizontalStartPoint, verticalStartPoint);
	m_weapon1Projectile->setVelocity(sf::Vector2f(0, yVel));
	return m_weapon1Projectile->clone();

}

std::optional<std::shared_ptr<Projectile>> Ship::fireWeapon2IfFired()
{
	return {};
}

void Ship::setColor(const sf::Color& color)
{
	Sprite::setColor(color);
}



