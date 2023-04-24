#include "PlayerShip.h"

#include "Controller.h"
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

void PlayerShip::handleIntro()
{
	if(GameState::gameCycleCounter > 24 && !GameState::isIntroDone)
	{
		if(!GameState::isKeyTrapActivated)
			disableCurrentShipStates();
		m_isVerticallyWorldBound = true;
		GameState::isIntroDone = true;
	}
}

void PlayerShip::updateShip(KeyboardController& controller, BoundedFloatRect worldBounds)
{
	handleIntro();
	bool swapControls = false;
	if (m_shipControlsStateMappings.at(Rotate) && !m_isTransitioning)//should be its own variable
		swapControls = true;
	Ship::updateShip(worldBounds);
	if(swapControls)
		controller.swapControlsAndStatesBasedOnMovementSetting(*this);
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