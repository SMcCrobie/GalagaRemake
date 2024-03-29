﻿/*
 * File:   collision.h
 * Authors: Nick Koirala (original version), ahnonay (SFML2 compatibility), switchboy (single pixel test)
 *          Paweł Syska (C++17 refactor + naming convention)
 *
 * Collision Detection and handling class
 * For SFML2.

Notice from the original version:

(c) 2009 - LittleMonkey Ltd

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
   you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment
   in the product documentation would be appreciated but
   is not required.

2. Altered source versions must be plainly marked as such,
   and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
   source distribution.

 *
 * Created on 30 January 2009, 11:02
 */

#pragma once
#include <optional>
#include <SFML/Graphics.hpp>

#include "Projectile.h"

namespace Collision {
	std::optional<sf::Vector2f> pixelPerfectTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2,
	                                             sf::Uint8 alphaLimit = 0);
	std::optional<sf::Vector2f> pixelPerfectTest(const sf::Sprite& sprite, const sf::RectangleShape& rect,
	                                             sf::Uint8 alphaLimit = 0);
	std::optional<sf::Vector2f> pixelPerfectTest(const sf::Sprite& sprite, const RectangleProjectile& recProjectile,
	                                             sf::Uint8 alphaLimit = 0);
	std::optional<sf::Vector2f> pixelPerfectTest(const sf::Sprite& sprite, const CircleProjectile& cirProjectile,
	                                             sf::Uint8 alphaLimit = 0);
	std::optional<sf::Vector2f> pixelPerfectTest(const CircleProjectile& circleProjectile,
	                                             const RectangleProjectile& recProjectile, sf::Uint8 alphaLimit = 0);
	std::optional<sf::Vector2f> pixelPerfectTest(const CircleProjectile& circleProjectile1,
	                                             const CircleProjectile& circleProjectile2, sf::Uint8 alphaLimit = 0);
	bool contains(const CircleProjectile& c, const sf::Vector2f& p);
}
