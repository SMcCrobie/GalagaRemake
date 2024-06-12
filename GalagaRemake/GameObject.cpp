#include "GameObject.h"

#include <iostream>
#include "GameState.h"
#include "Collision.h"
#include "DebugMacros.h"


GameObject::GameObject(): m_oscillationTimer(0), m_oscillationThreshold(0), m_angularVelocity(0), m_pointValue(0),
                          m_framesCount(0), m_currentFrame(0),
                          m_timeOfLastFrame(0), m_deltaBetweenFrames(0)
{
	m_isThereSprite = false;
	m_isThereRect = false;
	m_isThereCircle = false;
	m_isThereCircle = false;
}

//remove with collision box option
void GameObject::setSprite(const sf::Sprite& sprite, const char* spriteName, const bool withCollisionBox)
{
	m_sprite = sprite;
	m_isThereSprite = true;
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0f, m_sprite.getLocalBounds().height / 2.0f);
	m_localCenterOfMass = m_sprite.getOrigin();
	m_spriteName = spriteName;

	if(withCollisionBox)
	{
		setRect(sf::RectangleShape(sf::Vector2f(m_sprite.getLocalBounds().width, m_sprite.getLocalBounds().height)));
		m_rectangle.setPosition(m_sprite.getPosition());
	}
}

void GameObject::setRect(const sf::RectangleShape& rect)
{
	m_rectangle = rect;
	m_isThereRect = true;
	m_rectangle.setOrigin(m_rectangle.getLocalBounds().width / 2.0f, m_rectangle.getLocalBounds().height / 2.0f);
	m_localCenterOfMass = m_rectangle.getOrigin();
}

void GameObject::setCircle(const sf::CircleShape& circle)
{
	m_circle = circle;
	m_isThereCircle = true;
	m_circle.setOrigin(m_circle.getRadius(), m_circle.getRadius());
	m_localCenterOfMass = m_circle.getOrigin();
}

void GameObject::setRotation(const float rotation)
{
	m_angularVelocity = rotation;
}

void GameObject::setVelocity(const sf::Vector2f& velocity)
{
	m_velocity = velocity;
}

void GameObject::setVelocity(const float x, const float y)
{
	m_velocity.x = x;
	m_velocity.y = y;
}

void GameObject::setScale(const float x, const float y)
{
	if (m_isThereSprite)
	{
		m_sprite.setScale(x, y);
	}
	if (m_isThereCircle)
	{
		m_circle.setScale(x, y);
	}
	if (m_isThereRect)
	{
		m_rectangle.setScale(x, y);
	}
}

sf::Vector2f GameObject::getVelocity() const
{
	return m_velocity;
}

void GameObject::setOscillation(const sf::Vector2f& scalar, const int framesTillSwitch)
{
	m_scalar = scalar;
	m_oscillationThreshold = framesTillSwitch;
	m_oscillationTimer = 0;

}

void GameObject::setPosition(const float x, const float y)
{
	if (m_isThereSprite)
	{
		m_sprite.setPosition(x, y);
	}
	if (m_isThereCircle)
	{
		m_circle.setPosition(x, y);
	}
	if (m_isThereRect)
	{
		m_rectangle.setPosition(x, y);
	}
}

sf::Vector2f GameObject::getPosition() const
{
	if (m_isThereSprite)
	{
		return m_sprite.getPosition();
	}
	if (m_isThereCircle)
	{
		return m_circle.getPosition();
	}
	if (m_isThereRect)
	{
		return m_rectangle.getPosition();
	}

	return {};
}

void GameObject::setPointValue(const int pointValue)
{
	m_pointValue = pointValue;
}


void GameObject::rotateObject()
{
	if (m_angularVelocity == 0.0f)
		return;
	if (m_isThereSprite)
	{
		m_sprite.rotate(m_angularVelocity);
	}
	if (m_isThereCircle)
	{
		m_circle.rotate(m_angularVelocity);
	}
	if (m_isThereRect)
	{
		m_rectangle.rotate(m_angularVelocity);
	}
	

}

void GameObject::oscillateObject()
{
	if (m_scalar.x == 0.0f && m_scalar.y == 0.0f)
		return;

	if (m_isThereSprite)
	{
		m_sprite.scale(m_scalar);
	}
	if (m_isThereCircle)
	{
		m_circle.scale(m_scalar);
	}
	if (m_isThereRect)
	{
		m_rectangle.scale(m_scalar);
	}

	m_oscillationTimer++;
	if (m_oscillationTimer > m_oscillationThreshold)
	{
		m_scalar.x = 1.0f / m_scalar.x;
		m_scalar.y = 1.0f / m_scalar.y;
		m_oscillationTimer = 0;
	}

}


void GameObject::move()
{
	if(m_velocity.x == 0.0f && m_velocity.y == 0.0f)
		return;
	if (m_isThereSprite)
	{
		m_sprite.move(m_velocity);
	}
	if (m_isThereCircle)
	{
		m_circle.move(m_velocity);
	}
	if (m_isThereRect)
	{
		m_rectangle.move(m_velocity);
	}
}


void GameObject::update()
{
	move();
	rotateObject();
	oscillateObject();
	updateFrame();
}

bool GameObject::detectCollision(const PlayerShip& playerShip) const
{
	//refactor this to be like other detect collision
	if (m_isThereSprite && m_isThereRect)
	{
		if (Collision::pixelPerfectTest(playerShip, m_rectangle))
		{
			std::cout << "Collision Detected" << std::endl;
			return true;
		}
	}
	return false;
}

int GameObject::getPointValue() const
{
	return m_pointValue;
}

void GameObject::changeSpriteFrame(const sf::IntRect frame)
{
	m_sprite.setTextureRect(frame);
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0f, m_sprite.getLocalBounds().height / 2.0f);
	m_localCenterOfMass = m_sprite.getOrigin();
}

void GameObject::validateSufficientFramesInSingleRowSprite(const int amountOfFrames, const sf::IntRect frame) const
{
	if (m_sprite.getGlobalBounds().width >= frame.width * amountOfFrames)
		return;

	std::cout << "There is not enough frames in (" << m_spriteName << ") to support the animation request" << std::endl
		<< "Requested (" << amountOfFrames << ")" << frame.width << "px frames; Sprite only supports ("
		<< m_sprite.getGlobalBounds().width / static_cast<float>(frame.width) << ")" << frame.width << "px frames" << std::endl;
	ShowConsole();
	std::cin.get();
	abort();
	
}

void GameObject::animateOnTimeLoop(const int amountOfFrames, const int deltaBetweenFrames, const sf::IntRect frame)
{
	
	validateSufficientFramesInSingleRowSprite(amountOfFrames, frame);

	changeSpriteFrame(frame);
	m_framesCount = amountOfFrames;
	m_deltaBetweenFrames = deltaBetweenFrames;
}


void GameObject::updateFrame()
{
	if(m_framesCount == 0 && m_deltaBetweenFrames == 0)
		return;

	if(GameState::gameCycleCounter - m_timeOfLastFrame < m_deltaBetweenFrames)
		return;

	m_timeOfLastFrame = GameState::gameCycleCounter;
	m_currentFrame++;
	if (m_currentFrame >= m_framesCount)
		m_currentFrame = 0;

	const auto frame = m_sprite.getTextureRect();
	m_sprite.setTextureRect(sf::IntRect(sf::Vector2i(frame.width* m_currentFrame, frame.top), sf::Vector2i(frame.width, frame.height)));

}

void GameObject::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	if(m_isThereSprite)
	{
		target.draw(m_sprite, states);
		return;
	}
	if(m_isThereCircle)
	{
		target.draw(m_circle, states);
		return;
	}
	if (m_isThereRect) {
		target.draw(m_rectangle, states);
	}
}

