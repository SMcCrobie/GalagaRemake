#include <cstdlib> 
#include <ctime>
#include <iostream>
#include <list>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <climits>

#include "BackgroundManager.h"
#include "BoundedFloatRect.h"
#include "Controller.h"
#include "ControllerConfigs.h"
#include "DebugMacros.h"
#include "GameState.h"
#include "Level.h"
#include "PlayerShip.h"
#include "Projectile.h"
#include "ProjectileManager.h"
#include "RandMacros.h"
#include "Ship.h"
#include "ShipManager.h"
#include "UIManager.h"
#include "Fonts.h"
#include "Loader.h"
#include "resource1.h"

//YOU HAVE TODOS TODO

#define SCORE_VALUE_AS_INT ((GameState::killCounter * 100))
#define GAME_SPEED 22

//Global Variables
BoundedFloatRect WORLD_BOUNDS(0.0f, 0.0f, 600.0f, 1000.0f);
sf::View WORLD_VIEW(WORLD_BOUNDS);


void posePlayerQuestion()
{
	using namespace std;
	std::cout << "Please Select a Movement Control Option\n(enter associated number)" << endl;
	std::cout << "[1] Full Window Orientation" << endl;
	std::cout << "[2] Ship Vertical Orientation with Window Horizontal Orientation(recommended)" << endl;
	std::cout << "[3] Full Ship Orientation" << endl;
}

void pollForMovementSetting(sf::RenderWindow& window)
{
	if (GameState::isMovementSet)
		return;

	using namespace std;
	ShowConsole();

	while (true) {
		posePlayerQuestion();
		char playerInput = cin.get();
		while(cin.get() != '\n') {}
		const int intValue = playerInput - '1';

		switch (intValue)
		{
		case GameState::movement_control::full_ship_orientation:
		case GameState::movement_control::window_and_ship_orientation:
		case GameState::movement_control::full_window_orientation:
			GameState::movementControlSetting = static_cast<GameState::movement_control>(intValue);
			GameState::isMovementSet = true;
			#ifdef NDEBUG
				HideConsole();
			#endif
			#ifdef _DEBUG
				ShowConsole();
			#endif
			window.requestFocus();
			//GameState::init();
			return;
		default:
			cout << "Not a valid option." << endl;
		}
	}
};

int main(int, char const**)
{

	HINSTANCE hInstance = GetModuleHandle(NULL);
	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(GetConsoleWindow(), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

#ifdef NDEBUG
	HideConsole();
#endif
#ifdef _DEBUG
	ShowConsole();
#endif
	
	srand(static_cast<unsigned int>(time(nullptr)));

	// X goes right and Y goes down
	// Y is inverted 0 at the top 1000 at the bottoma
	sf::RenderWindow window(sf::VideoMode(static_cast<int>(WORLD_BOUNDS.width),
		static_cast<int>(WORLD_BOUNDS.height)), "Galaga!");
	window.setKeyRepeatEnabled(false);
	//window.setView(WORLD_VIEW);
	window.display();

	//textures
	sf::Texture shipAnimations;
	sf::Texture bossAnimations;
	sf::Texture bossSideKicksAnimations;
	sf::Texture planetsSheet;

	auto bossProjectileTexture = std::make_shared<sf::Texture>();
	//bossProjectileTexture->setSmooth(true);
	auto meteorTexture = std::make_shared<sf::Texture>();

	//LOADING
	try {
		//textures
		Loader::LOAD_SAFELY(shipAnimations, "ShipAnimations.png");
		Loader::LOAD_SAFELY(bossAnimations, "bossAnimations.png");
		Loader::LOAD_SAFELY(bossSideKicksAnimations, "BossSideKicksAnimations.png");
		Loader::LOAD_SAFELY(*bossProjectileTexture, "shieldWithCracksOverTime.png");
		Loader::LOAD_SAFELY(planetsSheet, "Planets(1).png");
		Loader::LOAD_SAFELY(*meteorTexture, "meteor.png");

		//fonts
		Fonts::load();

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



	//tempt text
	TempText levelIntoTextPrimary("Level 1", Fonts::galaxus);
	TempText levelIntoTextSecondary("Entering Tarkion III Orbit", Fonts::playFair);

	TempText levelOutroTextPrimary("Level Complete", Fonts::galaxus);
	TempText levelOutroTextSecondary("Exiting Tarkion III Orbit", Fonts::playFair);

	//intialize ships
	auto playerShip = std::make_shared<PlayerShip>(shipAnimations, WORLD_BOUNDS);
	playerShip->setOrigin(playerShip->getLocalBounds().width / 2, playerShip->getLocalBounds().height / 2);

	Ship enemyShip;
	enemyShip.setIsHorizontallyWorldBound(false);
	enemyShip.setTexture(shipAnimations);
	enemyShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));
	enemyShip.setPosition(sf::Vector2f(300.f, -50.f));
	enemyShip.setShipColor(sf::Color::Magenta);
	enemyShip.setWeaponRechargeTime(20);
	enemyShip.setOrigin(enemyShip.getLocalBounds().width / 2, enemyShip.getLocalBounds().height / 2);

	Ship bossShip;
	bossShip.setIsHorizontallyWorldBound(false);
	bossShip.setTexture(bossAnimations);
	bossShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(42, 48)));
	bossShip.setPosition(sf::Vector2f(300.f, -150.f));
	bossShip.scale(2.f, 2.f);
	bossShip.setWeaponRechargeTime(15);

	Ship bossSideKicks;
	bossSideKicks.setIsHorizontallyWorldBound(false);
	bossSideKicks.setTexture(bossSideKicksAnimations);
	bossSideKicks.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));
	bossSideKicks.scale(1.2f, 1.2f);
	bossSideKicks.setHealth(15);
	//bossSideKicks.rotate180();
	bossSideKicks.setPosition(sf::Vector2f(150.f, 1050.f));
	bossSideKicks.setWeaponRechargeTime(15);
	


	CircleProjectile bossProjectile = CircleProjectile();
	bossProjectile.setVelocity(sf::Vector2f(0, -8));
	bossProjectile.setTexture(bossProjectileTexture.get());
	bossProjectile.setRadius(12.f);
	bossProjectile.setInitOffSets(12.f, 15.f);
	bossProjectile.setShieldColor(sf::Color::Red);
	bossProjectile.setTextureRect(sf::IntRect(0, 0, 64, 64));

	bossShip.setProjectile1(bossProjectile);

	auto shieldColor = sf::Color::Red;
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

	RectangleProjectile enemyProjectile = RectangleProjectile(sf::Vector2f(3.f, 12.f));
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


	std::map<State, std::vector<ShipControl>> bossSideKicksStateToShipControlInputsMap{ BOSS_SIDE_KICKS_STATE_TO_SHIP_CONTROL_INPUTS_MAP};
	std::map<State, std::vector<ShipControl>> bossSideKicksStateToShipControlInputsMapB{ BOSS_SIDE_KICKS_STATE_TO_SHIP_CONTROL_INPUTS_MAP_B};
	std::map<State, std::map<Input, State>> bossSideKicksStateWithInputToStateMap{ BOSS_SIDE_KICKS_STATE_WITH_INPUT_TO_STATE_MAP };
	auto bossSideKicksController = StateMachineController(bossSideKicksStateToShipControlInputsMap,
		bossSideKicksStateWithInputToStateMap, 3, 200);
	auto bossSideKicksControllerB = StateMachineController(bossSideKicksStateToShipControlInputsMapB,
		bossSideKicksStateWithInputToStateMap, 3, 200);


	std::map<State, std::vector<ShipControl>> bossStateToShipControlInputsMap{BOSS_STATE_TO_SHIP_CONTROL_INPUT_MAP};
	std::map<State, std::map<Input, State>> bossStateWithInputToStateMap{ BOSS_STATE_WITH_INPUT_TO_STATE_MAP };

	auto bossController = StateMachineController(bossStateToShipControlInputsMap, 
												 bossStateWithInputToStateMap, 4);


	//Managers Class intialization
	auto enemyProjectileManager = std::make_shared<ProjectileManager>();
	auto playerProjectileManager = std::make_shared<ProjectileManager>();
	auto enemyShipsManager = std::make_shared<ShipManager>();
	auto uiManager = std::make_shared<UIManager>(*playerShip, WORLD_BOUNDS);
	auto backgroundManager = std::make_shared<BackgroundManager>(WORLD_BOUNDS);

	uiManager->initializeLevelIntroText(levelIntoTextPrimary, levelIntoTextSecondary);
	backgroundManager->addForegroundPlanet(planet);


	//Level Manager
	Level level;
	level.addDrawableLayer(backgroundManager)
		 .addDrawableLayer(playerProjectileManager)
		 .addDrawableLayer(enemyProjectileManager)
		 .addDrawableLayer(enemyShipsManager)
		 .addDrawableLayer(playerShip)
		 .addDrawableLayer(uiManager);
	
	level.addManager(backgroundManager)
		 .addManager(playerProjectileManager)
		 .addManager(enemyProjectileManager)
		 .addManager(enemyShipsManager)
		 .addManager(playerShip)
		 .addManager(uiManager);


	//intialize controller
	KeyboardController playerController{};
	StateMachineController enemyController;

	float backgroundSpeed = .25f;
	GameState::init();

	while (window.isOpen())
	{ 

		// Update the window, 
		window.clear();
		window.draw(level);
		window.display();

		pollForMovementSetting(window);

		//Run game loop every X milliseconds
		if (GameState::clock.getElapsedTime().asMilliseconds() - GameState::timeOfLastGameLoop <= GAME_SPEED)
			continue;

		GameState::timeOfLastGameLoop = GameState::clock.getElapsedTime().asMilliseconds();
		
		//Poll for events
		playerController.PollEventsAndUpdateShipState(window, *playerShip);
		//enemyController.updateControllerStateAndShipState(enemyShip);

		if (level.checkForGameEvent())
			continue;
		
		//enemyShipCreation - currently based off time, should be based off gamecycles
		if (GameState::gameCycleCounter - GameState::timeOfLastEnemyShip >= GameState::deltaTillNextEnemyShip) {
			int size = enemyShipsManager->count();
			if (GameState::killCounter < 8 && enemyShipsManager->count() < 2  ) {
				float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 589.F);//should make sizing dynamic
				enemyShip.setPosition(sf::Vector2f(xCoordinate, WORLD_BOUNDS.top - 50.f));
				enemyShipsManager->createShip(enemyShip);
			}

			if (GameState::killCounter >= 8 && GameState::killCounter < 24 && enemyShipsManager->count() < 4) {
				float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 589.F);//should make sizing dynamic
				enemyShip.setPosition(sf::Vector2f(xCoordinate, WORLD_BOUNDS.top - 50.f));
				enemyShipsManager->createShip(enemyShip);
			}

			if (GameState::killCounter >= 24 && GameState::killCounter < 32 && enemyShipsManager->count() < INT_MAX) {
				float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 589.F);//should make sizing dynamic
				enemyShip.setPosition(sf::Vector2f(xCoordinate, WORLD_BOUNDS.top - 50.f));
				enemyShipsManager->createShip(enemyShip);
			} 
			//enemyship Upgrade
			if (GameState::killCounter >= 8 && GameState::killCounter < 24 && enemyShipsManager->count() < 4) {
				float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 546.F);
				enemyShip.setPosition(sf::Vector2f(xCoordinate, WORLD_BOUNDS.bottom + 50.f));
				enemyShip.rotate180();
				enemyShipsManager->createShip(enemyShip);
				enemyShip.rotate180();
			}
			if (GameState::killCounter >= 24 && GameState::killCounter < 32 && enemyShipsManager->count() < INT_MAX) {
				float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 546.F);
				enemyShip.setPosition(sf::Vector2f(xCoordinate, WORLD_BOUNDS.bottom + 50.f));
				enemyShip.rotate180();
				enemyShipsManager->createShip(enemyShip);
				enemyShip.rotate180();
			}
			//boss
			if (GameState::killCounter >= 32 && !GameState::isBossCreated && enemyShipsManager->isEmpty()) {
				GameState::isBossCreated = true;
				enemyShipsManager->createShip(bossShip, bossController);
				enemyShipsManager->createShip(bossSideKicks, bossSideKicksController);
				bossSideKicks.move(300.f, 0.f);
				enemyShipsManager->createShip(bossSideKicks, bossSideKicksControllerB);
				//this is why you need to standardize ship creation, prob set position before any creation period
				bossSideKicks.move(-300.f, 0.f);
			}
			GameState::timeOfLastEnemyShip = GameState::gameCycleCounter;
			if(size < enemyShipsManager->count())
				GameState::deltaTillNextEnemyShip -= 5;
		}
	

		//update ships based on inputs from controllers
		playerShip->updateShip(playerController, WORLD_BOUNDS);
		enemyShipsManager->updateShips(WORLD_BOUNDS);


		//updateBackground
		backgroundManager->moveBackground(backgroundSpeed);

		//projectile calls
		playerProjectileManager->collectProjectile(*playerShip);
		enemyShipsManager->offloadProjectiles(*enemyProjectileManager);

		playerProjectileManager->updateProjectiles(WORLD_BOUNDS);
		enemyProjectileManager->updateProjectiles(WORLD_BOUNDS);

		enemyShipsManager->detectCollision(*playerProjectileManager, GameState::killCounter);
		bool isOutOfLives = uiManager->isOutOfLives();
		if (!playerShip->isRespawning() && playerShip->detectCollision(*enemyProjectileManager)
			&& !playerShip->hasHealth()) {

			if (isOutOfLives) {
				GameState::isGameOver = true;
				uiManager->updateUI(SCORE_VALUE_AS_INT);
				continue;
			}
			uiManager->playerLostLife();
			playerShip->respawnShip();
		}
		if(GameState::isBossCreated && enemyShipsManager->isEmpty() && !GameState::isBossDestroyed)
		{
			GameState::killCounter += 8;
			GameState::isBossDestroyed = true;
			GameState::levelOutroDelay = 40;
		}
		if (GameState::levelOutroDelay > -20)
			GameState::levelOutroDelay--;
		if(GameState::levelOutroDelay == 0)
		{
			GameState::levelOutroDelay--;
			uiManager->initializeLevelOutroText(levelOutroTextPrimary, levelOutroTextSecondary);
		
		}

		//UI Update
		uiManager->updateUI(SCORE_VALUE_AS_INT);

		GameState::gameCycleCounter++;

	}
	return EXIT_SUCCESS;

}

