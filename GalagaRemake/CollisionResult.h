#pragma once
#include <SFML/System/Vector2.hpp>

struct CollisionResult
{
	sf::Vector2f momentum;
	sf::Vector2f pointOfImpact;
	float rotationalVelocity;
};

