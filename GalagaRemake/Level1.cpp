#include "Level1.h"

#include <climits>
#include <iostream>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "BackgroundManager.h"
#include "Controller.h"
#include "ControllerConfigs.h"
#include "Fonts.h"
#include "GameState.h"
#include "Loader.h"
#include "RandMacros.h"
#include "Ship.h"


//textures
static sf::Texture shipAnimations;
static sf::Texture bossAnimations;
static sf::Texture bossSideKicksAnimations;
static std::shared_ptr<sf::Texture> bossProjectileTexture;

//objects
static Ship enemyShip;
static Ship bossShip;
static Ship bossSideKicks;
static CircleProjectile bossProjectile = CircleProjectile();
static RectangleProjectile enemyProjectile = RectangleProjectile(sf::Vector2f(3.f, 12.f));
static sf::Color shieldColor;

//controllers
static std::map<State, std::vector<ShipControl>> bossSideKicksStateToShipControlInputsMap;
static std::map<State, std::vector<ShipControl>> bossSideKicksStateToShipControlInputsMapB;
static std::map<State, std::map<Input, State>> bossSideKicksStateWithInputToStateMap;
static StateMachineController bossSideKicksController;
static StateMachineController bossSideKicksControllerB;


static std::map<State, std::vector<ShipControl>> bossStateToShipControlInputsMap;
static std::map<State, std::map<Input, State>> bossStateWithInputToStateMap;

static StateMachineController bossController;

static sf::Texture planetsSheet;

int Level1::initializeLevel()
{
	bossProjectileTexture = std::make_shared<sf::Texture>();
	try {
		//textures
		Loader::LOAD_SAFELY(shipAnimations, "ShipAnimations.png");
		Loader::LOAD_SAFELY(bossAnimations, "bossAnimations.png");
		Loader::LOAD_SAFELY(bossSideKicksAnimations, "BossSideKicksAnimations.png");
		Loader::LOAD_SAFELY(*bossProjectileTexture, "shieldWithCracksOverTime.png");
		Loader::LOAD_SAFELY(planetsSheet, "Planets(1).png");

	}
	catch (std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}


	planetsSheet.setSmooth(true);
	sf::Sprite planet(planetsSheet);
	planet.setPosition(300, -500);
	planet.setColor(sf::Color(120, 120, 120));

	extern BackgroundManager backgroundManager;
	backgroundManager.addForegroundPlanet(planet);

	enemyShip.setIsHorizontallyWorldBound(false);
	enemyShip.setTexture(shipAnimations);
	enemyShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));
	enemyShip.setPosition(sf::Vector2f(300.f, -50.f));
	enemyShip.setShipColor(sf::Color::Magenta);
	enemyShip.setWeaponRechargeTime(20);
	enemyShip.setOrigin(enemyShip.getLocalBounds().width / 2, enemyShip.getLocalBounds().height / 2);


	bossShip.setIsHorizontallyWorldBound(false);
	bossShip.setTexture(bossAnimations);
	bossShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(42, 48)));
	bossShip.setPosition(sf::Vector2f(300.f, -150.f));
	bossShip.scale(2.f, 2.f);
	bossShip.setWeaponRechargeTime(15);


	bossSideKicks.setIsHorizontallyWorldBound(false);
	bossSideKicks.setTexture(bossSideKicksAnimations);
	bossSideKicks.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));
	bossSideKicks.scale(1.2f, 1.2f);
	bossSideKicks.setHealth(15);
	//bossSideKicks.rotate180();
	bossSideKicks.setPosition(sf::Vector2f(150.f, 1050.f));
	bossSideKicks.setWeaponRechargeTime(15);




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

	
	enemyProjectile.setFillColor(sf::Color::Magenta);
	enemyProjectile.setVelocity(sf::Vector2f(0, -8));

	enemyShip.setProjectile1(enemyProjectile);
	enemyShip.rotate180();
	enemyShip.setHealth(2);

	enemyProjectile.setFillColor(sf::Color::Red);
	enemyProjectile.setSize(sf::Vector2f(4.f, 16.f));
	enemyProjectile.setInitOffSets(15, 10);
	bossSideKicks.setProjectile1(enemyProjectile);

	enemyProjectile.setInitOffSets(-15, 10);
	bossSideKicks.setProjectile2(enemyProjectile);


	bossSideKicksStateToShipControlInputsMap = std::map<State, std::vector<ShipControl>>{ BOSS_SIDE_KICKS_STATE_TO_SHIP_CONTROL_INPUTS_MAP};
	bossSideKicksStateToShipControlInputsMapB = std::map<State, std::vector<ShipControl>>{ BOSS_SIDE_KICKS_STATE_TO_SHIP_CONTROL_INPUTS_MAP_B};
	bossSideKicksStateWithInputToStateMap = std::map<State, std::map<Input, State>>{ BOSS_SIDE_KICKS_STATE_WITH_INPUT_TO_STATE_MAP };
	bossSideKicksController = StateMachineController(bossSideKicksStateToShipControlInputsMap,
		bossSideKicksStateWithInputToStateMap, 3, 200);
	bossSideKicksControllerB = StateMachineController(bossSideKicksStateToShipControlInputsMapB,
		bossSideKicksStateWithInputToStateMap, 3, 200);


	bossStateToShipControlInputsMap = std::map<State, std::vector<ShipControl>>{BOSS_STATE_TO_SHIP_CONTROL_INPUT_MAP};
	bossStateWithInputToStateMap = std::map<State, std::map<Input, State>>{ BOSS_STATE_WITH_INPUT_TO_STATE_MAP };

	bossController = StateMachineController(bossStateToShipControlInputsMap,
		bossStateWithInputToStateMap, 4);
	level_into_text_primary = TempText("Level 1", Fonts::galaxus);
	level_into_text_secondary = TempText("Entering Tarkion III Orbit", Fonts::playFair);

	level_outro_text_primary = TempText("Level Complete", Fonts::galaxus);
	level_outro_text_secondary = TempText("Exiting Tarkion III Orbit", Fonts::playFair);


	return 0;
}

void Level1::enemyShipCreation()
{
	extern ShipManager enemyShipsManager;
	if (GameState::gameCycleCounter - GameState::timeOfLastEnemyShip <= GameState::deltaTillNextEnemyShip)
		return;

	int size = enemyShipsManager.count();
	if (GameState::killCounter < 8 && enemyShipsManager.count() < 2) {
		float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 589.F);//should make sizing dynamic
		enemyShip.setPosition(sf::Vector2f(xCoordinate, GameState::world_bounds.top - 50.f));
		enemyShipsManager.createShip(enemyShip);
	}

	if (GameState::killCounter >= 8 && GameState::killCounter < 24 && enemyShipsManager.count() < 4) {
		float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 589.F);//should make sizing dynamic
		enemyShip.setPosition(sf::Vector2f(xCoordinate, GameState::world_bounds.top - 50.f));
		enemyShipsManager.createShip(enemyShip);
	}

	if (GameState::killCounter >= 24 && GameState::killCounter < 32 && enemyShipsManager.count() < INT_MAX) {
		float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 589.F);//should make sizing dynamic
		enemyShip.setPosition(sf::Vector2f(xCoordinate, GameState::world_bounds.top - 50.f));
		enemyShipsManager.createShip(enemyShip);
	}
	//enemyship Upgrade
	if (GameState::killCounter >= 8 && GameState::killCounter < 24 && enemyShipsManager.count() < 4) {
		float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 546.F);
		enemyShip.setPosition(sf::Vector2f(xCoordinate, GameState::world_bounds.bottom + 50.f));
		enemyShip.rotate180();
		enemyShipsManager.createShip(enemyShip);
		enemyShip.rotate180();
	}
	if (GameState::killCounter >= 24 && GameState::killCounter < 32 && enemyShipsManager.count() < INT_MAX) {
		float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 546.F);
		enemyShip.setPosition(sf::Vector2f(xCoordinate, GameState::world_bounds.bottom + 50.f));
		enemyShip.rotate180();
		enemyShipsManager.createShip(enemyShip);
		enemyShip.rotate180();
	}
	//boss
	if (GameState::killCounter >= 32 && !GameState::isBossCreated && enemyShipsManager.isEmpty()) {
		GameState::isBossCreated = true;
		enemyShipsManager.createShip(bossShip, bossController);
		enemyShipsManager.createShip(bossSideKicks, bossSideKicksController);
		bossSideKicks.move(300.f, 0.f);
		enemyShipsManager.createShip(bossSideKicks, bossSideKicksControllerB);
		//this is why you need to standardize ship creation, prob set position before any creation period
		bossSideKicks.move(-300.f, 0.f);
	}
	GameState::timeOfLastEnemyShip = GameState::gameCycleCounter;
	if (size < enemyShipsManager.count())
		GameState::deltaTillNextEnemyShip -= 5;
	
}


void Level1::killLevel()
{
	return;
}

void Level1::resetLevel()
{
};
