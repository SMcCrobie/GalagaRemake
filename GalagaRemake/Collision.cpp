/*
 * File:   collision.cpp
 * Author: Nick (original version), ahnonay (SFML2 compatibility), Paweł Syska (C++17 refactor + naming convention)
 */
#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include "Collision.h"

namespace Collision
{
	using TextureMask = std::vector<sf::Uint8>;

	static sf::Uint8 getPixel(const TextureMask& mask, const sf::Texture& tex, uint32_t x, uint32_t y) {
		if (x > tex.getSize().x || y > tex.getSize().y)
			return 0;

		return mask[x + y * tex.getSize().x];
	}

	class BitmaskRegistry
	{
	public:

		std::vector<sf::Uint8>& get(const sf::Texture& tex) {
			auto pair = bitmasks.find(&tex);
			if (pair == bitmasks.end())
			{
				return create(tex, tex.copyToImage());
			}

			return pair->second;
		}

		std::vector<sf::Uint8>& create(const sf::Texture& tex, const sf::Image& img) {
			auto mask = TextureMask(tex.getSize().y * tex.getSize().x);

			for (uint32_t y = 0; y < tex.getSize().y; ++y)
			{
				for (uint32_t x = 0; x < tex.getSize().x; ++x)
					mask[x + y * tex.getSize().x] = img.getPixel(x, y).a;
			}

			// store and return ref to the mask
			return (bitmasks[&tex] = std::move(mask));
		}
	private:
		std::map<const sf::Texture*, TextureMask> bitmasks;
	};

	// Gets global instance of BitmaskRegistry.
	// "static" to make sure this function doesn't leak to other source file
	static BitmaskRegistry& bitmasks() {
		static BitmaskRegistry instance;
		return instance;
	}


	bool pixelPerfectTest(const sf::Sprite& sprite, const sf::RectangleShape& rect, sf::Uint8 alphaLimit)
	{
		sf::FloatRect intersection;
		if (!sprite.getGlobalBounds().intersects(rect.getGlobalBounds(), intersection))
			return false;

		const auto s1SubRect = sprite.getTextureRect();

		const auto& mask1 = bitmasks().get(*sprite.getTexture());

		// Loop through our pixels
		for (auto i = intersection.left; i < intersection.left + intersection.width; ++i) {
			for (auto j = intersection.top; j < intersection.top + intersection.height; ++j) {
				const auto s1v = sprite.getInverseTransform().transformPoint(i, j);

				// Make sure pixels fall within the sprite's subrect
				if (s1v.x > 0 && s1v.y > 0 && s1v.x < s1SubRect.width && s1v.y < s1SubRect.height) {

					if (getPixel(mask1, *sprite.getTexture(), static_cast<int>(s1v.x) + s1SubRect.left, static_cast<int>(s1v.y) + s1SubRect.top) > alphaLimit)
						return true;

				}
			}
		}
		return false;
	}


	bool pixelPerfectTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2 , sf::Uint8 alphaLimit)
	{
		sf::FloatRect intersection;
		if (!sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds(), intersection))
			return false;

		const auto s1SubRect = sprite1.getTextureRect();
		const auto s2SubRect = sprite2.getTextureRect();

		const auto& mask1 = bitmasks().get(*sprite1.getTexture());
		const auto& mask2 = bitmasks().get(*sprite2.getTexture());

		// Loop through our pixels
		for (auto i = intersection.left; i < intersection.left + intersection.width; ++i) {
			for (auto j = intersection.top; j < intersection.top + intersection.height; ++j) {
				const auto s1v = sprite1.getInverseTransform().transformPoint(i, j);
				const auto s2v = sprite2.getInverseTransform().transformPoint(i, j);

				// Make sure pixels fall within the sprite's subrect
				if ((s1v.x > 0 && s1v.y > 0 && s1v.x < s1SubRect.width && s1v.y < s1SubRect.height)
					&& (s2v.x > 0 && s2v.y > 0 && s2v.x < s2SubRect.width && s2v.y < s2SubRect.height)) {

					if (getPixel(mask1, *sprite1.getTexture(), static_cast<int>(s1v.x) + s1SubRect.left, static_cast<int>(s1v.y) + s1SubRect.top) > alphaLimit
						&& getPixel(mask2, *sprite2.getTexture(), static_cast<int>(s2v.x) + s2SubRect.left, static_cast<int>(s2v.y) + s2SubRect.top) > alphaLimit)
						return true;

				}
			}
		}
		return false;
	}

	bool pixelPerfectTest(const sf::Sprite& sprite, const RectangleProjectile& recProjectile, sf::Uint8 alphaLimit)
	{
		sf::FloatRect intersection;
		if (!sprite.getGlobalBounds().intersects(recProjectile.getGlobalBounds(), intersection))
			return false;

		const auto s1SubRect = sprite.getTextureRect();

		const auto& mask1 = bitmasks().get(*sprite.getTexture());

		// Loop through our pixels
		for (auto i = intersection.left; i < intersection.left + intersection.width; ++i) {
			for (auto j = intersection.top; j < intersection.top + intersection.height; ++j) {
				const auto s1v = sprite.getInverseTransform().transformPoint(i, j);

				// Make sure pixels fall within the sprite's subrect
				if (s1v.x > 0 && s1v.y > 0 && s1v.x < s1SubRect.width && s1v.y < s1SubRect.height) {

					if (getPixel(mask1, *sprite.getTexture(), static_cast<int>(s1v.x) + s1SubRect.left, static_cast<int>(s1v.y) + s1SubRect.top) > alphaLimit)
						return true;

				}
			}
		}
		return false;
	}

	bool pixelPerfectTest(const sf::Sprite& sprite, const CircleProjectile& cirProjectile, sf::Uint8 alphaLimit)
	{
		sf::FloatRect intersection;
		if (!sprite.getGlobalBounds().intersects(cirProjectile.getGlobalBounds(), intersection))
			return false;

		const auto s1SubRect = sprite.getTextureRect();

		const auto& mask1 = bitmasks().get(*sprite.getTexture());

		// Loop through our pixels
		for (auto i = intersection.left; i < intersection.left + intersection.width; ++i) {
			for (auto j = intersection.top; j < intersection.top + intersection.height; ++j) {
				const auto s1v = sprite.getInverseTransform().transformPoint(i, j);

				// Make sure pixels fall within the sprite's subrect
				if (s1v.x > 0 && s1v.y > 0 &&
					s1v.x < s1SubRect.width && s1v.y < s1SubRect.height) {

					if (getPixel(mask1, *sprite.getTexture(), static_cast<int>(s1v.x) + s1SubRect.left, static_cast<int>(s1v.y) + s1SubRect.top) > alphaLimit
						&& contains(cirProjectile, sf::Vector2f(i,j) ) )
						return true;

				}
			}
		}
		return false;
	}

	bool pixelPerfectTest(const CircleProjectile& circleProjectile, const RectangleProjectile& recProjectile,
		sf::Uint8 alphaLimit)
	{
		sf::FloatRect fr = recProjectile.getGlobalBounds();
		sf::Vector2f topLeft(fr.left, fr.top);
		sf::Vector2f topRight(fr.left + fr.width, fr.top);
		sf::Vector2f botLeft(fr.left, fr.top + fr.height);
		sf::Vector2f botRight(fr.left + fr.width, fr.top + fr.height);

		return contains(circleProjectile, topLeft) ||
			contains(circleProjectile, topRight) ||
			contains(circleProjectile, botLeft) ||
			contains(circleProjectile, botRight);
	}

	bool pixelPerfectTest(const CircleProjectile& circleProjectile1, const CircleProjectile& circleProjectile2,
		sf::Uint8 alphaLimit)
	{
		//TODO	
		return false;
	}

	bool contains(const CircleProjectile& c, const sf::Vector2f& p)
	{
		sf::Vector2f center = c.getPosition();
		float a = (p.x - center.x);
		float b = (p.y - center.y);
		a *= a;
		b *= b;
		float r = c.getRadius() * c.getRadius();

		return ((a + b) < r);
	}
}
 