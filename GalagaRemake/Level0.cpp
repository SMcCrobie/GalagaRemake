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


static int timeOfLastEnemyShip = 0;
static int deltaTillNextEnemyShip = 0;
static bool isBossCreated = false;
static bool isRepairPackCreated = false;


static Ship bossShip;
static sf::Texture bossAnimations;
static auto laserTurretProjectile = RectangleProjectile();
static std::shared_ptr<sf::Texture> bossProjectileTexture;
static sf::Color shieldColor;

static Collidable meteor;
static sf::Sprite meteorSprite;
static sf::Texture meteorTexture;

static Item repairKit;
static sf::Sprite repairKitSprite;
static sf::Texture repairKitAnimations;

static Ship laserTurret;
static sf::Texture laserTurretAnimations;
static StateMachineController laserTurretController;

static std::map<State, std::vector<ShipControl>> bossStateToShipControlInputsMap;
static std::map<State, std::map<Input, State>> bossStateWithInputToStateMap;

static std::map<State, std::vector<ShipControl>> laserTurretStateToShipControlInputsMap;
static std::map<State, std::map<Input, State>> laserTurretStateWithInputToStateMap;



int Level0::initializeLevel()
{
	extern BackgroundManager backgroundManager; 
	extern PlayerShip playerShip;
	backgroundManager.removeForegroundPlanet();
	timeOfLastEnemyShip = 0;
	deltaTillNextEnemyShip = 100;
	isBossCreated = false;

	bossProjectileTexture = std::make_shared<sf::Texture>();
	try {
		Loader::LOAD_SAFELY(bossAnimations, "bossAnimations.png");
		Loader::LOAD_SAFELY(laserTurretAnimations, "LaserTurretAnimations.png");
		Loader::LOAD_SAFELY(*bossProjectileTexture, "shieldWithCracksOverTime.png");
		Loader::LOAD_SAFELY(meteorTexture, "meteor.png");

		Loader::LOAD_SAFELY(repairKitAnimations, "repairKit3.png");
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

	laserTurret.setTexture(laserTurretAnimations);
	laserTurret.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)));
	laserTurret.setPosition(sf::Vector2f(100.f, 1040.f));
	laserTurret.setWeaponRechargeTime(8);
	laserTurret.setIsHorizontallyWorldBound(false);
	laserTurret.setMovementIncrements(0.f, 0.05f, 0.05f);
	laserTurret.setHealth(10);


	laserTurretProjectile.setFillColor(sf::Color::Red);
	laserTurretProjectile.setSize(sf::Vector2f(20.f, 10.f));
	laserTurretProjectile.setInitOffSets(12.f, 10.f);
	laserTurretProjectile.setVelocity(sf::Vector2f(4.f, 0.f));



	laserTurretStateToShipControlInputsMap = std::map<State, std::vector<ShipControl>>{ LASER_TURRET_STATE_TO_SHIP_CONTROL_INPUTS_MAP };
	laserTurretStateWithInputToStateMap = std::map<State, std::map<Input, State>>{ LASER_TURRET_STATE_WITH_INPUT_TO_STATE_MAP };
	laserTurretController = StateMachineController(laserTurretStateToShipControlInputsMap,
		laserTurretStateWithInputToStateMap, 2, 200);

	laserTurret.setProjectile1(laserTurretProjectile);


	repairKitSprite.setTexture(repairKitAnimations);
	repairKitSprite.setPosition(sf::Vector2f(300.f, 100.f));
	repairKitSprite.setScale(.8f, .8f);
	repairKit.setSprite(repairKitSprite);
	repairKit.setRotation(-1.5f);
	repairKit.setOscillation(sf::Vector2f(1.004f, 1.004f), 80);
	repairKit.setItemType(ItemType::Repair_Kit);

	meteorSprite.setTexture(meteorTexture);
	meteor.setSprite(meteorSprite, false);
	meteor.setHealth(5);
	meteor.setVelocity(0.3f, 1.5f);
	meteor.setRotation(1.8f);
	


	playerShip.setHealth(1);
	





	return 0;
}

void Level0::enemyShipCreation()
{
	extern ShipManager enemyShipsManager;
	extern GameObjectManager gameObjectManager;

	if (GameState::gameCycleCounter - GameState::timeOfLastEnemyShip <= GameState::deltaTillNextEnemyShip)
		return;
	GameState::timeOfLastEnemyShip = GameState::gameCycleCounter;

	if(gameObjectManager.itemsCount() == 0 && !isRepairPackCreated)
	{
		isRepairPackCreated = true;
		gameObjectManager.createItem(repairKit);
	}
	if(gameObjectManager.collidablesCount() < 100)
	{
		const float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 589.F);
		const float yVel = RANDOM_FLOAT_WITHIN_LIMIT(.1f, 2.f);
		const float xVel = RANDOM_FLOAT_WITHIN_LIMIT(-.5f, .5f);
		const float rotation = RANDOM_FLOAT_WITHIN_LIMIT(-2.5f, 2.5f);
		const float scale = RANDOM_FLOAT_WITHIN_LIMIT(.4f, 2.5f);
		const float mass = 15.f * (scale*6.f);


		meteor.setPosition(xCoordinate, -10);
		meteor.setVelocity(xVel, yVel);
		meteor.setRotation(rotation);
		meteor.setScale(scale, scale);
		meteor.setHealth(static_cast<int>(std::round(scale * 5)));
		meteor.setMass(mass);


		gameObjectManager.createCollidable(meteor);
	}
	/*
	if (enemyShipsManager.count() < 2) {
		laserTurret.setPosition(sf::Vector2f(GameState::world_bounds.left + 50.f, GameState::world_bounds.top - 50.f));
		enemyShipsManager.createShip(laserTurret, laserTurretController);
	}*/
}

void Level0::killLevel()
{

}

void Level0::resetLevel()
{
	extern PlayerShip playerShip;
	playerShip.setHealth(1);
	isRepairPackCreated = false;
}
