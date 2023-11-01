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
#include "LevelOrchestrator.h"
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
#include "WindowUtil.h"

#include <cstdio>
#include <conio.h>

#include "GameObjectManager.h"
#include "Level0.h"
#include "Level1.h"
//YOU HAVE TODOS TODO

#define SCORE_VALUE_AS_INT ((GameState::killCounter * 100))

//Managers Class declaration
PlayerShip playerShip;
ProjectileManager enemyProjectileManager;
ProjectileManager playerProjectileManager;
ShipManager enemyShipsManager;
UIManager uiManager;
BackgroundManager backgroundManager;
GameObjectManager gameObjectManager;

//Level Orchestrator
LevelOrchestrator levelOrchestrator;

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
	Window::create_window_dynamically(window);
	window.display();

	//textures
	sf::Texture shipAnimations;

	auto meteorTexture = std::make_shared<sf::Texture>();

	//LOADING
	try {
		//textures
		Loader::LOAD_SAFELY(shipAnimations, "ShipAnimations.png");
		Loader::LOAD_SAFELY(*meteorTexture, "meteor.png");
		//fonts
		Fonts::load();

	}
	catch (std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}

	//intialize ships
	playerShip.init(shipAnimations);
	uiManager.init(playerShip);
	playerShip.setOrigin(playerShip.getLocalBounds().width / 2, playerShip.getLocalBounds().height / 2);


	//intialize controller
	KeyboardController playerController{};
	StateMachineController enemyController;

	float backgroundSpeed = .25f;//move inside backgroundmanager
	GameState::init();

	std::shared_ptr<ILevel> level0 = std::make_shared<Level0>();
	std::shared_ptr<ILevel> level1 = std::make_shared<Level1>();











	//current way to hotswap any level in
	/////////////////////////////////////////////////////////////
	levelOrchestrator.loadLevel(level0);
	/////////////////////////////////////////////////////////////














	levelOrchestrator.initDefaultManager();
	levelOrchestrator.initDefaultDrawableLayersAndOrder();
	levelOrchestrator.initializeLevelIntroText(uiManager);

	while (window.isOpen())
	{ 
		levelOrchestrator.updateWindow(window);
		if(levelOrchestrator.shouldRunLoop() == false)
			continue;

		if (GameState::isMovementSet == false)
			levelOrchestrator.pollForMovementSetting(window);

		//Poll for events
		playerController.PollEventsAndUpdateShipState(window, playerShip);

		if (levelOrchestrator.checkForGameEvent(playerController))
			continue;

		levelOrchestrator.enemyShipCreation();

		//update ships based on inputs from controllers
		playerShip.updateShip(playerController);
		enemyShipsManager.updateShips();

		//update Game objects
		gameObjectManager.updateObjects();

		//updateBackground
		backgroundManager.moveBackground(backgroundSpeed);

		//projectile calls
		playerProjectileManager.collectProjectile(playerShip);
		enemyShipsManager.offloadProjectiles(enemyProjectileManager);

		playerProjectileManager.updateProjectiles();
		enemyProjectileManager.updateProjectiles();
		
		enemyShipsManager.detectCollision(playerProjectileManager);
		bool isOutOfLives = uiManager.isOutOfLives();
		if (!playerShip.isRespawning() && playerShip.detectCollision(enemyProjectileManager)
			&& !playerShip.hasHealth()) {

			if (isOutOfLives) {
				GameState::isGameOver = true;
				uiManager.updateUI(SCORE_VALUE_AS_INT);
				continue;
			}
			uiManager.playerLostLife();
			playerShip.respawnShip();
		}
		if(GameState::isBossCreated && enemyShipsManager.isEmpty() && !GameState::isBossDestroyed)
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
			levelOrchestrator.initializeLevelOutroText(uiManager);
		
		}

		//UI Update
		uiManager.updateUI(SCORE_VALUE_AS_INT);

		GameState::gameCycleCounter++;

	}
	return EXIT_SUCCESS;

}

