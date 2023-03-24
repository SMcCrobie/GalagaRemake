#include <cstdlib>
#include <cstdlib> 
#include <ctime>
#include <iostream>
#include <list>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "BackgroundManager.h"
#include "BoundedFloatRect.h"
#include "Controller.h"
#include "ControllerConfigs.h"
#include "DebugMacros.h"
#include "PlayerShip.h"
#include "Projectile.h"
#include "ProjectileManager.h"
#include "RandMacros.h"
#include "Ship.h"
#include "ShipManager.h"
#include "UIManager.h"

template<typename LoaderClass>
void LOAD_SAFELY(LoaderClass& loader, const std::string& filePath)
{
	if (!loader.loadFromFile(filePath))
	{
		ShowConsole();
		throw std::invalid_argument("Failed to load " + filePath);
	}
}

#define SCORE_VALUE_AS_INT ((gameCycleCounter/20) + (killCounter * 100))

//Global Variables
BoundedFloatRect WORLD_BOUNDS(0.0f, 0.0f, 600.0f, 1000.0f);
sf::View WORLD_VIEW(WORLD_BOUNDS);


int main(int, char const**)
{
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
	window.setView(WORLD_VIEW);

	//Clock
	sf::Clock clock;

	//textures
	sf::Texture shipAnimations;
	sf::Texture bossAnimations;
	sf::Texture bossSideKicksAnimations;
	sf::Texture planetsSheet;

	auto bossProjectileTexture = std::make_shared<sf::Texture>();
	bossProjectileTexture->setSmooth(true);
	auto meteorTexture = std::make_shared<sf::Texture>();

	//fonts
	sf::Font font;


	//LOADING
	try {
		//textures
		LOAD_SAFELY(shipAnimations, "ShipAnimations.png");
		LOAD_SAFELY(bossAnimations, "bossAnimations.png");
		LOAD_SAFELY(bossSideKicksAnimations, "BossSideKicksAnimations.png");
		LOAD_SAFELY(*bossProjectileTexture, "shieldWithCracksOverTime.png");
		LOAD_SAFELY(planetsSheet, "Planets(2).png");
		LOAD_SAFELY(*meteorTexture, "meteor.png");

		//Fonts
		LOAD_SAFELY(font, "ClimateCrisis-Regular.ttf");
	}
	catch (std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	planetsSheet.setSmooth(true);
	sf::Sprite planet(planetsSheet);
	planet.setPosition(300, -500);
	planet.setColor(sf::Color(120, 120, 120));



	//intialize ships
	PlayerShip playerShip(shipAnimations, WORLD_BOUNDS);

	Ship enemyShip;
	enemyShip.setIsHorizontallyWorldBound(false);
	enemyShip.setTexture(shipAnimations);
	enemyShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));
	enemyShip.setPosition(sf::Vector2f(300.f, -50.f));
	enemyShip.setShipColor(sf::Color::Magenta);

	Ship bossShip;
	bossShip.setIsHorizontallyWorldBound(false);
	bossShip.setTexture(bossAnimations);
	bossShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(42, 48)));
	bossShip.setPosition(sf::Vector2f(300.f, -150.f));
	bossShip.scale(2.f, 2.f);

	Ship bossSideKicks;
	bossSideKicks.setIsHorizontallyWorldBound(false);
	bossSideKicks.setTexture(bossSideKicksAnimations);
	bossSideKicks.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));
	bossSideKicks.setPosition(sf::Vector2f(150.f, 1050.f));
	bossSideKicks.scale(1.2f, 1.2f);
	bossSideKicks.setHealth(15);
	bossSideKicks.rotate180();


	CircleProjectile bossProjectile = CircleProjectile();
	bossProjectile.setVelocity(sf::Vector2f(0, -8));
	bossProjectile.setTexture(bossProjectileTexture.get());
	bossProjectile.setRadius(12.f);
	bossProjectile.setShieldColor(sf::Color::Red);
	bossProjectile.setTextureRect(sf::IntRect(0, 0, 64, 64));

	bossShip.setProjectile1(bossProjectile);

	auto shieldColor = sf::Color::Red;
	shieldColor.a = 100;

	auto radius = 56.f;
	bossProjectile.setRadius(radius);
	bossProjectile.setShieldColor(shieldColor);
	bossProjectile.setOrigin(radius, radius);
	bossShip.setHealth(30);
	bossShip.setShield(bossProjectile,50);
	bossShip.rotate180();

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


	std::map<State, std::vector<ShipControl>> bossStateToShipControlInputsMap{BOSS_STATE_TO_SHIP_CONTROL_INPUT_MAP_CONFIG};
	std::map<State, std::map<Input, State>> bossStateWithInputToStateMap{ BOSS_STATE_WITH_INPUT_TO_STATE_MAP_CONFIG };

	auto bossController = StateMachineController(bossStateToShipControlInputsMap, 
												 bossStateWithInputToStateMap, 4);


	//Managers Class intialization
	ProjectileManager enemyProjectileManager;
	ProjectileManager playerProjectileManager;
	ShipManager enemyShipsManager;
	UIManager uiManager(playerShip, font, WORLD_BOUNDS);
	BackgroundManager backgroundManager(WORLD_BOUNDS);
	backgroundManager.addForegroundPlanet(planet);
	
	//intialize controller
	KeyboardController playerController{};
	StateMachineController enemyController;

	//counters
	int timeOfLastGameLoop = 0;
	int timeOfLastEnemyShip = -1000;
	int deltaTillNextEnemyShip = 6000;
	int killCounter = 0;
	int gameCycleCounter = 0;

	bool isPaused = false;
	bool isPausedPressed = false;
	bool isGameOver = false;	
	bool isBossCreated = false;

	while (window.isOpen())
	{ 

		// Update the window, 
		window.clear();
		window.draw(backgroundManager);
		window.draw(playerProjectileManager);
		window.draw(enemyProjectileManager);
		window.draw(playerShip);
		window.draw(enemyShipsManager);
		window.draw(uiManager);
		window.display();

		//Run game loop every X milliseconds
		if (clock.getElapsedTime().asMilliseconds() - timeOfLastGameLoop <= 20) {
			continue;
		}
		timeOfLastGameLoop = clock.getElapsedTime().asMilliseconds();
		
		//Poll for events
		isPausedPressed = playerController.PollEventsAndUpdateShipState(window, playerShip);
		enemyController.updateControllerStateAndShipState(clock, enemyShip);

		if (isGameOver) {
			continue;
		}
		
		//Pause check
		if (isPausedPressed)
			isPaused = !isPaused;
		if (isPaused)
			continue;

		
		//enemyShipCreation - currently based off time, should be based off gamecycles
		if (clock.getElapsedTime().asMilliseconds() - timeOfLastEnemyShip >= deltaTillNextEnemyShip) {
			if (killCounter <= 30) {
				float xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 589.F);//should make sizing dynamic
				enemyShip.setPosition(sf::Vector2f(xCoordinate, WORLD_BOUNDS.top - 50.f));
				enemyShipsManager.createShip(enemyShip);


				//enemyship Upgrade
				if (killCounter > 8) {
					xCoordinate = RANDOM_FLOAT_WITHIN_LIMIT(56.F, 589.F);
					enemyShip.setPosition(sf::Vector2f(xCoordinate, WORLD_BOUNDS.bottom + 50.f));
					enemyShip.rotate180();
					enemyShipsManager.createShip(enemyShip);
					enemyShip.rotate180();
				}
			}
			if (killCounter > 30 && !isBossCreated && enemyShipsManager.isEmpty()) {
				isBossCreated = true;
				enemyShipsManager.createShip(bossShip, bossController);
				enemyShipsManager.createShip(bossSideKicks, bossSideKicksController);
				bossSideKicks.move(300.f, 0.f);
				enemyShipsManager.createShip(bossSideKicks, bossSideKicksControllerB);
			}

			timeOfLastEnemyShip = clock.getElapsedTime().asMilliseconds();
			deltaTillNextEnemyShip = deltaTillNextEnemyShip - 40;
		}
	
		//apply texture, based on events from player controller
		playerShip.setTextureRectBasedOnShipState();

		//update ships based on inputs from controllers
		playerShip.updateShip(WORLD_BOUNDS);
		enemyShipsManager.updateShips(WORLD_BOUNDS, clock);


		//updateBackground
		backgroundManager.moveBackground(.25f);

		//projectile calls
		playerProjectileManager.collectProjectile(playerShip);
		enemyShipsManager.offloadProjectiles(enemyProjectileManager);

		playerProjectileManager.updateProjectiles(WORLD_BOUNDS);
		enemyProjectileManager.updateProjectiles(WORLD_BOUNDS);

		enemyShipsManager.detectCollision(playerProjectileManager, killCounter);
		bool isOutOfLives = uiManager.isOutOfLives();
		if (!playerShip.isRespawning() && playerShip.detectCollision(enemyProjectileManager)
			&& !playerShip.hasHealth()) {

			if (isOutOfLives) {
				isGameOver = true;
				uiManager.gameOver();
				continue;
			}
			uiManager.playerLostLife();
			playerShip.respawnShip();
		}

		//UI Update
		uiManager.updateUI(SCORE_VALUE_AS_INT);

		gameCycleCounter++;

	}
	return EXIT_SUCCESS;

}

