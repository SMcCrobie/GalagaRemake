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
#include "DebugMacros.h"
#include "PlayerShip.h"
#include "Projectile.h"
#include "ProjectileManager.h"
#include "Ship.h"
#include "ShipManager.h"
#include "UIManager.h"


#define SCORE_VALUE_AS_INT ((gameCycleCounter/20) + (killCounter * 100))
//#define Seans_Debug

//Global Variables
BoundedFloatRect WORLD_BOUNDS(0.0f, 0.0f, 600.0f, 1000.0f);
sf::View WORLD_VIEW(WORLD_BOUNDS);


int main(int, char const**)
{
	//HideConsole();
	//ShowConsole();
#ifdef NDEBUG
	HideConsole();
#endif
#ifdef _DEBUG
	ShowConsole();
#endif

	srand(static_cast<unsigned int>(time(nullptr)));
	// X goes right and Y goes down
	// Y is inverted 0 at the top 1000 at the bottom
	sf::RenderWindow window(sf::VideoMode(static_cast<int>(WORLD_BOUNDS.width), 
		static_cast<int>(WORLD_BOUNDS.height)), "Galaga!");
	window.setKeyRepeatEnabled(false);
	window.setView(WORLD_VIEW);

	//Clock
	sf::Clock clock;

	//textures
	sf::Texture shipAnimations;
	if (!shipAnimations.loadFromFile("ShipAnimations.png"))
	{
		std::cout << "failed to load ShipAnimations.png" << std::endl;
		return EXIT_FAILURE;
	}
	sf::Texture bossAnimations;
	if (!bossAnimations.loadFromFile("bossAnimations.png"))
	{
		std::cout << "failed to load bossAnimations.png" << std::endl;
		return EXIT_FAILURE;
	}
	sf::Texture planetsSheet;
	if (!planetsSheet.loadFromFile("Planets(1).png"))
	{
		std::cout << "failed to load PLanets(1).png" << std::endl;
		return EXIT_FAILURE;
	}
	//Fonts
	sf::Font font;
	if (!font.loadFromFile("ClimateCrisis-Regular.ttf"))
	{
		std::cout << "failed to load ClimateCrisis-Regular.ttf" << std::endl;
		return EXIT_FAILURE;
	}


	planetsSheet.setSmooth(true);
	sf::Sprite planet(planetsSheet);
	//planet.setTextureRect(sf::IntRect(sf::Vector2i(100, 0), sf::Vector2i(100, 100)));
	planet.setPosition(300, -500);
	//planet.setScale(2.5f, 2.5f);
	planet.setColor(sf::Color(120, 120, 120));
	




	//intialize ships
	PlayerShip playerShip(shipAnimations, WORLD_BOUNDS);

	Ship enemyShip;
	enemyShip.setIsHorizontallyWorldBound(false);
	enemyShip.setTexture(shipAnimations);
	enemyShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));
	enemyShip.setPosition(sf::Vector2f(300.f, -50.f));
	enemyShip.setColor(sf::Color::Magenta);

	Ship bossShip;
	bossShip.setIsHorizontallyWorldBound(false);
	bossShip.setTexture(bossAnimations);
	bossShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(42, 48)));
	bossShip.setPosition(sf::Vector2f(300.f, -50.f));
	auto preScaleSize = bossShip.getGlobalBounds();
	bossShip.scale(2.f, 2.f);
	auto postScaleSize = bossShip.getGlobalBounds();
	//bossShip.setColor(sf::Color::Magenta);

	CircleProjectile bossProjectile = CircleProjectile();
	bossProjectile.setFillColor(sf::Color::Red);
	bossProjectile.setVelocity(sf::Vector2f(0, -8));
	bossProjectile.setRadius(10.f);
	//bossProjectile.setOutlineColor(sf::Color::Cyan);
	//bossProjectile.setOutlineThickness(2.f);

	bossShip.setProjectile(std::make_shared<CircleProjectile>(bossProjectile));
	bossShip.rotate180();


	
	RectangleProjectile enemyProjectile = RectangleProjectile(sf::Vector2f(3.f, 12.f));
	enemyProjectile.setFillColor(sf::Color::Magenta);
	/*enemyProjectile.setOutlineColor(sf::Color(255, 255, 255, 30));
	enemyProjectile.setOutlineThickness(2.f);*/
	enemyProjectile.setVelocity(sf::Vector2f(0, -8));
	enemyShip.setProjectile(std::make_shared<Projectile>(enemyProjectile));
	//enemyShip.setScale(1.4f, .6f);
	enemyShip.rotate180();

	//Managers Class intialization
	ProjectileManager enemyProjectileManager;
	ProjectileManager playerProjectileManager;
	ShipManager enemyShipsManager;
	UIManager uiManager(playerShip, font, WORLD_BOUNDS);
	BackgroundManager backgroundManager(WORLD_BOUNDS);
	backgroundManager.addForegroundPlanet(planet);
	
	//intialize controller
	KeyboardController playerController;
	StateMachineController enemyController;

	//counters
	int timeOfLastGameLoop = 0;
	int timeOfLastEnemyShip = -1000;
	int deltaTillNextEnemyShip = 6000;
	int killCounter = 31;
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

		//Run game loop every 25 milliseconds
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

		
		//enemyShipCreation
		if (clock.getElapsedTime().asMilliseconds() - timeOfLastEnemyShip >= deltaTillNextEnemyShip) {
			if (killCounter <= 30) {
				float shipWidth = enemyShip.getGlobalBounds().width;//somehow this is wrong
				float xCoordinate = 56.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (589.f - 56.f)));
				enemyShip.setPosition(sf::Vector2f(xCoordinate, WORLD_BOUNDS.top - 50.f));
				enemyShipsManager.createShip(enemyShip);


				//enemyship Upgrade
				if (killCounter > 8) {
					float xCoordinate = 56.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (589.f - 56.f)));
					enemyShip.setPosition(sf::Vector2f(xCoordinate, WORLD_BOUNDS.bottom + 50.f));
					enemyShip.rotate180();
					enemyShipsManager.createShip(enemyShip);
					enemyShip.rotate180();
				}
			}
			if (killCounter > 30 && !isBossCreated) {
				isBossCreated = true;
				enemyShipsManager.createShip(bossShip);
			}

			timeOfLastEnemyShip = clock.getElapsedTime().asMilliseconds();
			deltaTillNextEnemyShip = deltaTillNextEnemyShip - 40;
		}
	
		//apply texture, based on events from player controller
		playerShip.setTextureRectBasedOnShipState();
		//playerShip.updateShadingIfRespawning();

		//Apply inputs and envirmental factors to movement
		playerShip.updateShip(WORLD_BOUNDS);
		playerShip.moveShip();

		playerShip.rotateIfTriggered();

		//UI Update
		uiManager.updateUI(SCORE_VALUE_AS_INT);

		//updateBackground
		backgroundManager.moveBackground(.25f);

		//projectile calls
		playerProjectileManager.collectProjectile(playerShip);
		enemyShipsManager.offloadProjectiles(enemyProjectileManager);

		playerProjectileManager.updateProjectiles(WORLD_BOUNDS);
		enemyProjectileManager.updateProjectiles(WORLD_BOUNDS);
		enemyShipsManager.updateShips(WORLD_BOUNDS, clock);

		enemyShipsManager.detectCollision(playerProjectileManager, killCounter);
		bool isOutOfLives = uiManager.isOutOfLives();
		if (!playerShip.isRespawning() && playerShip.detectCollision(enemyProjectileManager)) {
			if (isOutOfLives) {
				isGameOver = true;
				uiManager.gameOver();
				continue;
			}
			uiManager.playerLostLife();
			playerShip.respawnShip();
		}		
		gameCycleCounter++;

	}

	return EXIT_SUCCESS;

}

