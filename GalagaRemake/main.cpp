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
#include "LevelManager.h"
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


#include <cstdio>
#include <conio.h>

#include "Level0.h"
#include "Level1.h"
//YOU HAVE TODOS TODO

#define SCORE_VALUE_AS_INT ((GameState::killCounter * 100))


void create_window_dynamically(sf::RenderWindow& window)
{
	unsigned int screenHeight = sf::VideoMode::getDesktopMode().height / 10 * 7;//70% of screen height
	window.create(sf::VideoMode(screenHeight / 5 * 3, screenHeight), "Galaga!");
	sf::View view(sf::FloatRect(0.f, 0.f, GameState::world_bounds.width, GameState::world_bounds.height));
	view.setViewport(sf::FloatRect(0, 0, 1, 1)); // full viewport
	window.setView(view);
	window.setKeyRepeatEnabled(false);
}

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
	// Y is inverted 0 at the top 1000 at the bottoman
	sf::RenderWindow window;
	create_window_dynamically(window);
	window.display();

	//textures
	sf::Texture shipAnimations;
	sf::Texture planetsSheet;

	auto meteorTexture = std::make_shared<sf::Texture>();

	//LOADING
	try {
		//textures
		Loader::LOAD_SAFELY(shipAnimations, "ShipAnimations.png");
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


	//intialize ships
	auto playerShip = std::make_shared<PlayerShip>(shipAnimations);
	playerShip->setOrigin(playerShip->getLocalBounds().width / 2, playerShip->getLocalBounds().height / 2);

	//Managers Class intialization
	auto enemyProjectileManager = std::make_shared<ProjectileManager>();
	auto playerProjectileManager = std::make_shared<ProjectileManager>();
	auto enemyShipsManager = std::make_shared<ShipManager>();
	auto uiManager = std::make_shared<UIManager>(*playerShip);
	auto backgroundManager = std::make_shared<BackgroundManager>();

	backgroundManager->addForegroundPlanet(planet);


	//Level Manager
	LevelManager levelManager;
	levelManager.addDrawableLayer(backgroundManager)
		 .addDrawableLayer(playerProjectileManager)
		 .addDrawableLayer(enemyProjectileManager)
		 .addDrawableLayer(enemyShipsManager)
		 .addDrawableLayer(playerShip)
		 .addDrawableLayer(uiManager);
	
	levelManager.addManager(backgroundManager)
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

	std::shared_ptr<ILevel> level0 = std::make_shared<Level0>();
	std::shared_ptr<ILevel> level1 = std::make_shared<Level1>();

	//current way to hotswap any level in
	levelManager.loadLevel(level0);


	levelManager.initializeLevelIntroText(*uiManager);

	while (window.isOpen())
	{ 
		levelManager.updateWindow(window);
		if(levelManager.shouldRunLoop() == false)
			continue;

		if (GameState::isMovementSet == false)
			levelManager.pollForMovementSetting(window);

		//Poll for events
		playerController.PollEventsAndUpdateShipState(window, *playerShip);

		if (levelManager.checkForGameEvent(playerController))
			continue;

		levelManager.enemyShipCreation(enemyShipsManager);

		//update ships based on inputs from controllers
		playerShip->updateShip(playerController);
		enemyShipsManager->updateShips();


		//updateBackground
		backgroundManager->moveBackground(backgroundSpeed);

		//projectile calls
		playerProjectileManager->collectProjectile(*playerShip);
		enemyShipsManager->offloadProjectiles(*enemyProjectileManager);

		playerProjectileManager->updateProjectiles();
		enemyProjectileManager->updateProjectiles();

		enemyShipsManager->detectCollision(*playerProjectileManager);
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
			levelManager.initializeLevelOutroText(*uiManager);
		
		}

		//UI Update
		uiManager->updateUI(SCORE_VALUE_AS_INT);

		GameState::gameCycleCounter++;

	}
	return EXIT_SUCCESS;

}

