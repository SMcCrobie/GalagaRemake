#include "Level0.h"

#include <iostream>

#include "Fonts.h"
#include "GameState.h"
#include "Loader.h"


int Level0::initializeLevel()
{
	timeOfLastEnemyShip = 0;
	deltaTillNextEnemyShip = 200;
	isBossCreated = false;

	bossProjectileTexture = std::make_shared<sf::Texture>();
	try {
		Loader::LOAD_SAFELY(bossAnimations, "bossAnimations.png");
		Loader::LOAD_SAFELY(*bossProjectileTexture, "shieldWithCracksOverTime.png");
	}
	catch (std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}


	level_into_text_primary = TempText("Level 0", Fonts::galaxus);
	level_into_text_secondary = TempText("Escaping Starship SaberII", Fonts::playFair);

	level_outro_text_primary = TempText("Level Complete", Fonts::galaxus);
	level_outro_text_secondary = TempText("Escaped Starship SaberII", Fonts::playFair);


	bossShip.setIsHorizontallyWorldBound(false);
	bossShip.setTexture(bossAnimations);
	bossShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(42, 48)));
	bossShip.setPosition(sf::Vector2f(300.f, 1050.f));
	bossShip.scale(2.f, 2.f);
	bossShip.setWeaponRechargeTime(15);

	bossProjectile.setVelocity(sf::Vector2f(0, -8));
	bossProjectile.setTexture(bossProjectileTexture.get());
	bossProjectile.setRadius(12.f);
	bossProjectile.setInitOffSets(12.f, 15.f);
	bossProjectile.setShieldColor(sf::Color::Red);
	bossProjectile.setTextureRect(sf::IntRect(0, 0, 64, 64));


	bossShip.setProjectile1(bossProjectile);

	shieldColor = sf::Color::Red;
	shieldColor.a = 125;

	auto radius = 56.f;
	bossProjectile.setRadius(radius);
	bossProjectile.setShieldColor(shieldColor);
	/*bossProjectile.setOutlineColor(sf::Color(0xa91e25ff));
	bossProjectile.setOutlineThickness(2.5f);*/
	bossProjectile.setOrigin(radius, radius);
	bossShip.setHealth(30);
	bossShip.rotate180();
	bossShip.setOrigin(bossShip.getLocalBounds().width / 2, bossShip.getLocalBounds().height / 2);
	bossShip.setShield(bossProjectile, 50);

	return 0;
}

void Level0::enemyShipCreation(std::shared_ptr<ShipManager>& enemyShipsManager)
{
	if (GameState::gameCycleCounter - timeOfLastEnemyShip <= deltaTillNextEnemyShip ||
		isBossCreated)
		return;
	isBossCreated = true;
	//enemyShipsManager->createShip(bossShip);

	return;
}

void Level0::killLevel()
{
	return;
}
