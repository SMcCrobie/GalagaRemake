#include "PlayerShip.h"

#include "Controller.h"
#include "GameState.h"

void PlayerShip::initStartHealth()
{

	m_shipHealth = 5;
}

void PlayerShip::initStartState()
{
	if (isBackwards())
		rotate180();
	m_gameCyclesTillRespawned = 0;
	setShipColor(sf::Color::White);
	m_velocity = sf::Vector2f(0.0f, -6.0f);

	initStartHealth();
	setPosition(m_startPosition);
	disableCurrentShipStates();
	m_isVerticallyWorldBound = false;
	m_shipControlsStateMappings[MoveUp] = true;
	m_horizontalDirectionIncrement = HORIZONTAL_DIRECTION_INCREMENT;
	m_moveUpIncrement = SINGLE_THRUST_DIRECTION_INCREMENT;
	m_moveDownIncrement = SINGLE_THRUST_DIRECTION_INCREMENT / 2;
}

void PlayerShip::calculateStartPosition()
{
	const sf::FloatRect shipSize = getGlobalBounds();
	const float xPos = (GameState::world_bounds.width / 2.f) - (shipSize.width / 2.f);
	const float yPos = GameState::world_bounds.height + 50.f;
	m_startPosition = sf::Vector2f(xPos, yPos);
}

PlayerShip::PlayerShip(const sf::Texture& texture)
	: Ship()
{
	setTexture(texture);
	setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));

	calculateStartPosition();
	initStartState();
}

PlayerShip::PlayerShip()
= default;

void PlayerShip::respawnShip()
{
	initStartHealth();
	m_gameCyclesTillRespawned = 100;
}

void PlayerShip::init(const sf::Texture& texture)
{
	m_startHealth = 3;
	setTexture(texture);
	setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));

	calculateStartPosition();
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

void PlayerShip::updateShip(KeyboardController& controller)
{
	handleIntro();
	bool swapControls = false;
	if (m_shipControlsStateMappings.at(Rotate) && !m_isTransitioning)//should be its own variable
		swapControls = true;
	Ship::updateShip(GameState::world_bounds);
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

void PlayerShip::useItem(const ItemType itemType)
{
	if (itemType == ItemType::None)
		return;
	if (itemType == ItemType::Repair_Kit)
	{
		initStartHealth();
		return;
	}
	throw std::invalid_argument("Item Type Not Mapped for Ship interaction");
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