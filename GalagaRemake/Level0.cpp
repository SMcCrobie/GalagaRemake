#include "Level0.h"

#include <iostream>

#include "BackgroundManager.h"
#include "Fonts.h"
#include "GameState.h"
#include "Loader.h"
#include "RandMacros.h"
#include "ControllerConfigs.h"
#include "GameObject.h"
#include "GameObjectManager.h"


static Collidable meteor;
static sf::Sprite meteorSprite;
static sf::Texture meteorTexture;

extern BackgroundManager backgroundManager;
extern PlayerShip playerShip;

int Level0::initializeLevel()
{
	backgroundManager.removeForegroundPlanet();

	try {
		Loader::LOAD_SAFELY(meteorTexture, "meteor.png");
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

	meteorSprite.setTexture(meteorTexture);
	meteor.setSprite(meteorSprite, false);
	meteor.setHealth(5);
	meteor.setVelocity(0.3f, 1.5f);
	meteor.setRotation(1.8f);
	





	return 0;
}

void Level0::enemyShipCreation()
{
	extern ShipManager enemyShipsManager;
	extern GameObjectManager gameObjectManager;

	if (GameState::gameCycleCounter - GameState::timeOfLastEnemyShip <= GameState::deltaTillNextEnemyShip-250)
		return;
	GameState::timeOfLastEnemyShip = GameState::gameCycleCounter;

	
	if(gameObjectManager.collidablesCount() < 100)
	{
		const float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 589.F);
		const float yVel = RANDOM_FLOAT_WITHIN_LIMIT(.8f, 2.f);
		const float xVel = RANDOM_FLOAT_WITHIN_LIMIT(-.5f, .5f);
		const float rotation = RANDOM_FLOAT_WITHIN_LIMIT(-2.5f, 2.5f);
		const float scale = RANDOM_FLOAT_WITHIN_LIMIT(.6f, 2.5f);
		const float mass = std::pow(scale * 10.6f, 1.55f);

		meteor.setRotation(rotation);
		meteor.setScale(scale, scale);
		meteor.setHealth(static_cast<int>(std::round(scale * 14)));
		meteor.setMass(mass);

		if(GameState::gameCycleCounter % 2 == 0)
		{
			meteor.setPosition(xCoordinate, -30);
			meteor.setVelocity(xVel, yVel);
			gameObjectManager.createCollidable(meteor);
		}
		if(GameState::gameCycleCounter % 2 != 0 )
		{
			meteor.setPosition(xCoordinate, 1030);
			meteor.setVelocity(-xVel, -yVel);
			gameObjectManager.createCollidable(meteor);
		}

	}
	
}

void Level0::killLevel()
{

}

void Level0::resetLevel()
{
	
}
