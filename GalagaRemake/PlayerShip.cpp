#include "PlayerShip.h"
#include "GameState.h"

void PlayerShip::initStartState()
{
	if (isBackwards())
		rotate180();
	m_gameCyclesTillRespawned = 0;
	setShipColor(sf::Color::White);
	m_velocity = sf::Vector2f(0.0f, -6.0f);
	m_shipHealth = 3;
	setPosition(m_startPosition);
	disableCurrentShipStates();
	m_isVerticallyWorldBound = false;
	m_shipControlsStateMappings[MoveUp] = true;
}

void PlayerShip::calculateStartPosition(const BoundedFloatRect& worldDimensions)
{
	const sf::FloatRect shipSize = getGlobalBounds();
	const float xPos = (worldDimensions.width / 2.f) - (shipSize.width / 2.f);
	const float yPos = 1050.f;//worldDimensions.height / 3 * 2;
	m_startPosition = sf::Vector2f(xPos, yPos);
}

PlayerShip::PlayerShip(const sf::Texture& texture, const BoundedFloatRect& worldDimensions)
	: Ship()
{
	setTexture(texture);
	setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));

	calculateStartPosition(worldDimensions);
	initStartState();
}

void PlayerShip::updateShip(BoundedFloatRect worldBounds)
{
	if(GameState::gameCycleCounter > 24 && !GameState::isIntroDone)
	{
		if(!GameState::isKeyTrapActivated)
			disableCurrentShipStates();
		m_isVerticallyWorldBound = true;
		GameState::isIntroDone = true;
	}

	Ship::updateShip(worldBounds);
}

void PlayerShip::setTextureRectBasedOnShipState()
{
	if (m_gameCyclesTillRespawned == 100) {
		setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x, 0), m_shipAnimationFrame));
		return;
	}
	applyTexture();
}

void PlayerShip::testAndApplyCushion(BoundedFloatRect& shipBounds, BoundedFloatRect worldBounds, float cushion)
{
	worldBounds += cushion;
	float threshold = 2.f;

	if (m_velocity.y < threshold) {}
	else if (isShipwithBottomCushionAndMovingThatDirection(shipBounds, worldBounds)
		|| isShipWithinTopCusionAndMovingThatDirection(shipBounds, worldBounds)) {
		m_velocity.y = m_velocity.y * .72f;
		return;
	}
	if (m_velocity.x < threshold) {}
	else if (isWithinRightCusionAndMovingThatDirection(shipBounds, worldBounds)
		|| isWithinLeftCushionAndMovingThatDirection(shipBounds, worldBounds))
		m_velocity.x = m_velocity.x * .72f;

}

void PlayerShip::rotateIfTriggered()
{
	if (!m_shipControlsStateMappings.at(Rotate))
		return;
	m_shipControlsStateMappings[Rotate] = false;
	
	rotate180();
}

void PlayerShip::changeDirectionIncrementsBasedOnMovementControl()
{
	using namespace GameState;
	const auto temp = m_moveUpIncrement;
	switch (movementControlSetting)
	{
	case full_window_orientation:
		m_moveUpIncrement = m_moveDownIncrement;
		m_moveDownIncrement = temp;
		break;
	case full_ship_orientation:
		m_moveUpIncrement = -m_moveUpIncrement;
		m_moveDownIncrement = -m_moveDownIncrement;
		m_horizontalDirectionIncrement = -m_horizontalDirectionIncrement;
		break;
	case window_and_ship_orientation:
		m_moveUpIncrement = -m_moveUpIncrement;
		m_moveDownIncrement = -m_moveDownIncrement;
	}
}

void PlayerShip::rotate180()
{
	m_isBackwards = !m_isBackwards;
	changeDirectionIncrementsBasedOnMovementControl();
	
	m_weapon1Projectile->setVelocity(sf::Vector2f(0, -m_weapon1Projectile->getVelocity().y));
	rotate(180.f);//origin is now bottom right, but global bounds still correctly gives top and left
	const sf::FloatRect localBounds = getLocalBounds();
	if (m_isBackwards)
		move(localBounds.width, localBounds.height);
	else
		move(-localBounds.width, -localBounds.height);
}

void PlayerShip::resetManager()
{
	initStartState();
}


bool PlayerShip::isWithinLeftCushionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds)
{
	return shipBounds.left + m_velocity.x < worldBounds.left + WORLD_BOUNDS_MARGIN && m_velocity.x < 0;
}

bool PlayerShip::isWithinRightCusionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds)
{
	return shipBounds.right + m_velocity.x > worldBounds.right - WORLD_BOUNDS_MARGIN && m_velocity.x > 0;
}

bool PlayerShip::isShipWithinTopCusionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds)
{
	return shipBounds.top + m_velocity.y < worldBounds.top + WORLD_BOUNDS_MARGIN && m_velocity.y < 0;
}

bool PlayerShip::isShipwithBottomCushionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds)
{
	return shipBounds.bottom + m_velocity.y > worldBounds.bottom - WORLD_BOUNDS_MARGIN && m_velocity.y > 0;
}