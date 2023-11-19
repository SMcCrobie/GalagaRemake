#pragma once
#include <optional>
#include <SFML/System/Vector2.hpp>

struct CollisionResult
{
	std::optional<sf::Vector2f> impact;
};

