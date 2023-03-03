#include "PlayerShip.h"

PlayerShip::PlayerShip(const sf::Texture& texture, BoundedFloatRect worldDimensions)
	: Ship()
{
	setTexture(texture);
	setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));
	
	sf::FloatRect shipSize = getGlobalBounds();
	float xPos = (worldDimensions.width / 2.f) - (shipSize.width / 2.f);
	float yPos = 700.f;
	
	setPosition(xPos, yPos);
}

void PlayerShip::setTextureRectBasedOnShipState()
{
	if (m_gameCyclesTillRespawned == 100) {
		setTextureRect(sf::IntRect(sf::Vector2i(m_shipAnimationFrame.x, 0), m_shipAnimationFrame));
		return;
	}
	if (m_isBackwards) {
		applyBackwardsTexture();
	}
	else {
		applyStandardTexture();
	}
}

void PlayerShip::updateShipVelocity(BoundedFloatRect worldBounds)
{
	BoundedFloatRect shipBounds = getGlobalBounds();
	//apply resistence
	if (m_velocity.y != 0)
		m_velocity.y = m_velocity.y * RESISTENCE_MULTIPLIER;

	if (m_velocity.x != 0)
		m_velocity.x = m_velocity.x * RESISTENCE_MULTIPLIER;

	//apply thrust to shipVelocity, after State update, opposing Thrusts cancel
	if (m_isBackwards) {
		applyBackwardsVelocity();
	}
	else {
		applyStandardVelocity();
	}


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

		/*if (abs(shipBounds.top - worldBounds.top - WORLD_BOUNDS_MARGIN) < .01f
			|| (abs(shipBounds.bottom - worldBounds.bottom + WORLD_BOUNDS_MARGIN) < .01f)) {
			m_velocity.x = 0;
			break;
		}*/
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

	return;
	
}

void PlayerShip::rotateIfTriggered()
{
	if (!m_shipControlsStateMappings.at(Rotate))
		return;
	m_shipControlsStateMappings[Rotate] = false;
	rotate180();
}

void PlayerShip::rotate180()
{
	m_isBackwards = !m_isBackwards;
	
	m_weapon1Projectile.setVelocity(sf::Vector2f(0, -m_weapon1Projectile.getVelocity().y));
	rotate(180.f);//origin is now bottom right, but global bounds still correctly gives top and left
	sf::FloatRect localBounds = getLocalBounds();
	if (m_isBackwards)
		sf::Transformable::move(localBounds.width, localBounds.height);
	else
		sf::Transformable::move(-localBounds.width, -localBounds.height);
}
