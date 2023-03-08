
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <cstdlib>
#include "Ship.h"
#include "Projectile.h"
#include "BoundedFloatRect.h"
#include "Controller.h"
#include "ProjectileManager.h"
#include "ShipManager.h"
#include "PlayerShip.h"
#include "UIManager.h"
#include "BackgroundManager.h"
#include <Windows.h>


#define Score_Value_As_Int ((gameCycleCounter/20) + (killCounter * 100))
//#define Seans_Debug

#ifdef Seans_Debug
int allocation_counter = 0;

void* operator new(size_t size)
{
	//std::cout << "allocating  " << size << " bytes of memory" << std::endl;
	allocation_counter++;
	std::cout << "allocating, current counter: " << allocation_counter << std::endl;
	/*if (size > 1000)
		std::cout << "why???" << std::endl;*/
	void* p = malloc(size);
	return p;
}

void operator delete(void* p)
{
	//std::cout << "Deleting Memory" << std::endl;
	allocation_counter--;
	std::cout << "Deleting, current counter: " << allocation_counter << std::endl;
	free(p);
}

void* operator new[](size_t size)
{
	//std::cout << "allocating  " << size << " bytes of memory" << std::endl;
	/*if (size > 1000)
		std::cout << "why???" << std::endl;*/
	allocation_counter++;
	std::cout << "allocating, current counter: " << allocation_counter << std::endl;
	void* p = malloc(size);
	return p;
}

void operator delete[](void* p)
{
	allocation_counter--;
	std::cout << "Deleting, current counter: " << allocation_counter << std::endl;
	//std::cout << "Deleting Memory" << std::endl;
	free(p);
}

#endif // Seans_Debug

void ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}
void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

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

	srand((unsigned int)time(NULL));
	// X goes right and Y goes down
	// Y is inverted 0 at the top 1000 at the bottom
	sf::RenderWindow window(sf::VideoMode((int)WORLD_BOUNDS.width, (int)WORLD_BOUNDS.height), "Galaga!");
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
	//textures
	sf::Texture planetsSheet;
	if (!planetsSheet.loadFromFile("Planets(1).png"))
	{
		std::cout << "failed to load PLanets(1).png" << std::endl;
		return EXIT_FAILURE;
	}
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
	enemyShip.setIsWorldBound(false);
	enemyShip.setTexture(shipAnimations);
	enemyShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));
	enemyShip.setPosition(sf::Vector2f(300.f, -50.f));
	enemyShip.setColor(sf::Color::Magenta);

	Projectile enemyProjectile = Projectile(sf::Vector2f(3.f, 12.f));
	enemyProjectile.setFillColor(sf::Color::Magenta);
	/*enemyProjectile.setOutlineColor(sf::Color(255, 255, 255, 30));
	enemyProjectile.setOutlineThickness(2.f);*/
	enemyProjectile.setVelocity(sf::Vector2f(0, -8));
	enemyShip.setProjectile(enemyProjectile);
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
	int killCounter = 0;
	int gameCycleCounter = 0;

	bool isPaused = false;
	bool isPausedPressed = false;
	bool isGameOver = false;	

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
		if (clock.getElapsedTime().asMilliseconds() - timeOfLastEnemyShip >= deltaTillNextEnemyShip && killCounter < 30) {
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
		uiManager.updateUI(Score_Value_As_Int);

		//updateBackground
		backgroundManager.moveBackground(.25f);

		//projectile calls
		playerProjectileManager.collectProjectile(playerShip);
		enemyProjectileManager.collectProjectile(enemyShipsManager);

		playerProjectileManager.updateProjectiles(WORLD_BOUNDS);
		enemyProjectileManager.updateProjectiles(WORLD_BOUNDS);
		enemyShipsManager.updateShips(WORLD_BOUNDS, clock);

		playerProjectileManager.detectCollision(enemyShipsManager, killCounter);
		bool isOutOfLives = uiManager.isOutOfLives();
		if (!playerShip.isRespawning() && enemyProjectileManager.detectCollision(playerShip.getGlobalBounds(), !isOutOfLives)) {
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

