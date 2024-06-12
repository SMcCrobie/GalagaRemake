#include <cstdlib> 
#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <climits>

#include "BackgroundManager.h"
#include "Controller.h"
#include "DebugMacros.h"
#include "GameState.h"
#include "LevelOrchestrator.h"
#include "PlayerShip.h"
#include "ProjectileManager.h"
#include "Ship.h"
#include "ShipManager.h"
#include "UIManager.h"
#include "Fonts.h"
#include "Loader.h"
#include "resource1.h"
#include "WindowUtil.h"

#include <conio.h>

#include "GameObjectManager.h"
#include "Level0.h"
#include "Level1.h"
#include "TestLevel1.h"
//YOU HAVE TODOS TODO

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

	//LOADING
	//textures
	Loader::LOAD_SAFELY(shipAnimations, "ShipAnimations.png");
	//fonts
	Fonts::load();

	

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
	std::shared_ptr<ILevel> flightTester = std::make_shared<TestLevel1>();







	//current way to hotswap any level in
	/////////////////////////////////////////////////////////////
	levelOrchestrator.loadLevel(level1);
	/////////////////////////////////////////////////////////////



	levelOrchestrator.initDefaultManager();
	levelOrchestrator.initDefaultDrawableLayersAndOrder();
	levelOrchestrator.initializeLevelIntroText(uiManager);

	while (window.isOpen())
	{ 
		levelOrchestrator.updateWindow(window);
		if(LevelOrchestrator::shouldRunLoop() == false)
			continue;

		if (GameState::isMovementSet == false)
			LevelOrchestrator::pollForMovementSetting(window);

		if (GameState::resetWindow)
			Window::reset_window_size_dynamically(window);
		//Poll for events
		playerController.PollEventsAndUpdateShipState(window, playerShip);

		if (levelOrchestrator.checkForGameEvent(playerController))
			continue;

		levelOrchestrator.enemyShipCreation();

		//update ships based on inputs from controllers
		playerShip.updateShip(playerController);
		enemyShipsManager.updateShips();

		//update Game objects
		gameObjectManager.update();

		//object and Item Collisions
		gameObjectManager.detectItemCollision(playerShip);

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
				uiManager.updateUI();
				continue;
			}
			uiManager.playerLostLife();
			playerShip.respawnShip();
		}
		if(GameState::isBossCreated && enemyShipsManager.isEmpty() && !GameState::isBossDestroyed)
		{
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
		uiManager.updateUI();

		GameState::gameCycleCounter++;

	}
	return EXIT_SUCCESS;

}

