#include "Ship.h"

Ship::Ship() : sf::Sprite()
{
	m_velocity = sf::Vector2f();
	m_collisionBox = sf::RectangleShape();
	m_shipControlsStateMappings = std::map<ShipControl, bool>();
	m_shipAnimationFrame = sf::Vector2i(45, 48);
	m_weapon1Projectile = Projectile(sf::Vector2f(3, 12));
	m_weapon1Projectile.setFillColor(sf::Color(0x05ecf1ff));
	m_horizontalDirectionIncrement = HORIZONTAL_DIRECTION_INCREMENT;
	m_verticalDirectionIncrement = SINGLE_THRUST_DIRECTION_INCREMENT;
	m_isBackwards = false;
	m_isWorldBound = true;
	
	for (int i = MoveUp; i < FireWeapon2; i++) {
		m_shipControlsStateMappings[ShipControl(i)] = false;
	}
	
}


//currently requires a 3x3 sprite sheet, would be better to have it adapatable to sprite sheet if possible
void Ship::setTextureRectBasedOnShipState()
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

void Ship::setProjectile(const Projectile& projectile)
{
	m_weapon1Projectile = projectile;
}

void Ship::setIsWorldBound(bool isWorldBound)
{
	m_isWorldBound = isWorldBound;
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
	if (m_shipControlsStateMappings.at(MoveUp) && m_velocity.y >= -MAX_VERTICAL_SPEED)
		m_velocity.y += -m_verticalDirectionIncrement;
	if (m_shipControlsStateMappings.at(MoveDown) && m_velocity.y <= MAX_VERTICAL_SPEED)
		m_velocity.y += m_verticalDirectionIncrement / 3.f;



	if (m_shipControlsStateMappings.at(MoveLeft) && m_velocity.x >= -MAX_HORIZONTAL_SPEED)
		m_velocity.x += -m_horizontalDirectionIncrement;
	if (m_shipControlsStateMappings.at(MoveRight) && m_velocity.x <= MAX_HORIZONTAL_SPEED)
		m_velocity.x += m_horizontalDirectionIncrement;




	//WORLD BOUNDS, these are better but stil prob not best. Maybe derive sf::FloatRec and add bounds so you dont have to add the top and height or the left and width to get the right coordinate
	while (shipBounds.right + m_velocity.x > worldBounds.right - WORLD_BOUNDS_MARGIN
		|| shipBounds.left + m_velocity.x < worldBounds.left + WORLD_BOUNDS_MARGIN) {



		if (abs(shipBounds.right - worldBounds.right - WORLD_BOUNDS_MARGIN) < .01f
			|| (abs(shipBounds.left - worldBounds.left + WORLD_BOUNDS_MARGIN) < .01f)) {
			m_velocity.x = 0;
			break;
		}
		m_velocity.x = m_velocity.x * .5f;



	}

	if (!m_isWorldBound)
		return;

	while (shipBounds.bottom + m_velocity.y > worldBounds.bottom - WORLD_BOUNDS_MARGIN
		|| shipBounds.top + m_velocity.y < worldBounds.top + WORLD_BOUNDS_MARGIN) {


		if (shipBounds.bottom > worldBounds.bottom - WORLD_BOUNDS_MARGIN) {
			m_velocity.y = (worldBounds.bottom - WORLD_BOUNDS_MARGIN) - shipBounds.bottom;
			break;
		}
		if (shipBounds.top < worldBounds.top + WORLD_BOUNDS_MARGIN) {
			m_velocity.y = shipBounds.top - (worldBounds.top + WORLD_BOUNDS_MARGIN);
			break;
		}
		m_velocity.y = m_velocity.y * .5f;

	}

	return;
}

void Ship::move()
{
	sf::Sprite::move(m_velocity);
}

void Ship::rotate180()
{
	m_isBackwards = !m_isBackwards;
	m_horizontalDirectionIncrement = -m_horizontalDirectionIncrement;
	m_verticalDirectionIncrement = -m_verticalDirectionIncrement;
	m_weapon1Projectile.setVelocity(sf::Vector2f(0, PROJECTILE_START_SPEED));
	rotate(180.f);//origin is now bottom right, but global bounds still correctly gives top and left

}

const std::map<ShipControl, bool>& Ship::getShipControlStateMappings()
{
	return m_shipControlsStateMappings;
}

std::optional<Projectile> Ship::fireWeapon1IfFired()
{
	if (!m_shipControlsStateMappings[FireWeapon1])
		return {};
	BoundedFloatRect currentShipPosition = getGlobalBounds();
	float verticalStartPoint = m_isBackwards ? currentShipPosition.bottom - m_weapon1Projectile.getGlobalBounds().height : currentShipPosition.top;

	m_shipControlsStateMappings[FireWeapon1] = false;
	m_weapon1Projectile.setPosition(currentShipPosition.left + (currentShipPosition.width / 2) - 2, verticalStartPoint);
	return m_weapon1Projectile;

}

std::optional<Projectile> Ship::fireWeapon2IfFired()
{
	return {};
}



