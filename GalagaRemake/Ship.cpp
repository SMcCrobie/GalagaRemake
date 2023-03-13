#include "Ship.h"

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
	
	for (int i = MoveUp; i < InvalidShipControl; i++) {
		m_shipControlsStateMappings[static_cast<ShipControl>(i)] = false;
	}
	
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
	if (m_gameCyclesTillRespawned == 100) {
		updateShadingIfRespawning();
		m_gameCyclesTillRespawned--;
		return;
	}
	if (m_gameCyclesTillRespawned > 0) {
		updateShadingIfRespawning();
		m_gameCyclesTillRespawned--;
	}
}

void Ship::updateShip(BoundedFloatRect worldBounds)
{
	updateRespawnTimer();
	if(!m_isStatic)
		updateShipVelocity(worldBounds);
}

void Ship::setStatic()
{
	m_isStatic = true;
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

	return;
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
	sf::Sprite::move(m_velocity);
}

void Ship::rotate180()//need to call after setting projectile
{
	m_isBackwards = !m_isBackwards;
	m_horizontalDirectionIncrement = -m_horizontalDirectionIncrement;
	m_verticalDirectionIncrement = -m_verticalDirectionIncrement;
	
	
	m_weapon1Projectile->setVelocity(sf::Vector2f(0, -m_weapon1Projectile->getVelocity().y));
	rotate(180.f);//origin is now bottom right, but global bounds still correctly gives top and left
	const sf::FloatRect localBounds = getGlobalBounds(); 
	if(m_isBackwards)
		move(localBounds.width,localBounds.height);
	else
		move(-localBounds.width, -localBounds.height);

}

void Ship::respawnShip()
{
	m_gameCyclesTillRespawned = 100;
}

bool Ship::isBackwards()
{
	return m_isBackwards;
}

bool Ship::isRespawning()
{
	return m_gameCyclesTillRespawned > 0;
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
	const float verticalStartPoint = m_isBackwards ? currentShipPosition.bottom : currentShipPosition.top;

	m_shipControlsStateMappings[FireWeapon1] = false;
	m_weapon1Projectile->setPosition(currentShipPosition.left + (currentShipPosition.width / 2) - 2, verticalStartPoint);
	return m_weapon1Projectile->clone();

}

std::optional<std::shared_ptr<Projectile>> Ship::fireWeapon2IfFired()
{
	return {};
}



