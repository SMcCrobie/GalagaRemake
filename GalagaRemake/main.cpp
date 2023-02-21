
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

//Global Variables
BoundedFloatRect WORLD_BOUNDS(0.0f, 0.0f, 600.0f, 1000.0f);
sf::View WORLD_VIEW(WORLD_BOUNDS);

int main(int, char const**)
{
	srand(time(NULL));
	// X goes right and Y goes down
	// Y is inverted 0 at the top 1000 at the bottom
	sf::RenderWindow window(sf::VideoMode(600, 1000), "Galaga!");
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

	//intialize ship
	Ship playerShip;
	playerShip.setTexture(shipAnimations);
	playerShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));
	playerShip.setPosition(sf::Vector2f(300.f, 800.f));


	Ship enemyShip;
	enemyShip.setIsWorldBound(false);
	enemyShip.setTexture(shipAnimations);
	enemyShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));
	enemyShip.setPosition(sf::Vector2f(300.f, -50.f));
	enemyShip.setColor(sf::Color::Magenta);
	
	Projectile enemyProjectile = Projectile(sf::Vector2f(3.f, 12.f));
	enemyProjectile.setFillColor(sf::Color::Magenta);
	enemyShip.setProjectile(enemyProjectile);
	enemyShip.rotate180();
	
	//intialize controller
	KeyboardController playerController;
	StateMachineController enemyController;


	//counters
	sf::Int32 timeOfLastGameLoop = 0;
	sf::Int32 timeOfLastEnemyShip = 500;
	bool isPaused = false;
	bool isPausedPressed = false;


	//containers for drawables
	ProjectileManager enemyProjectileManager;
	ProjectileManager playerProjectileManager;
	ShipManager enemyShipsManager;
	
	

	while (window.isOpen())
	{
		if (clock.getElapsedTime().asMilliseconds() - timeOfLastGameLoop <= 25) {
			continue;
		}
		timeOfLastGameLoop = clock.getElapsedTime().asMilliseconds();

		//Poll for events
		isPausedPressed = playerController.PollEventsAndUpdateShipState(window, playerShip);
		enemyController.updateControllerStateAndShipState(clock, enemyShip);
		
		//Pause check
		if (isPausedPressed)
			isPaused = !isPaused;
		if (isPaused)
			continue;

		
		//enemyShipCreation
		if (clock.getElapsedTime().asMilliseconds() - timeOfLastEnemyShip >= 3000) {
			//either below is broken or my boundries are broken... prob the latter
			float xCoordinate = 56.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (589.f - 56.f)));
			enemyShip.setPosition(sf::Vector2f(xCoordinate, WORLD_BOUNDS.top - 50.f));
			enemyShipsManager.createShip(enemyShip);
			timeOfLastEnemyShip = clock.getElapsedTime().asMilliseconds();
		}
		


		//increment the world
		WORLD_VIEW.move(0.f, WORLD_VIEW_MOVEMENT);
		WORLD_BOUNDS.top += WORLD_VIEW_MOVEMENT;
		WORLD_BOUNDS.bottom += WORLD_VIEW_MOVEMENT;
		window.setView(WORLD_VIEW);


		//apply texture, based on events from player controller
		playerShip.setTextureRectBasedOnShipState();

		//Apply inputs and envirmental factors to movement
		playerShip.updateShipVelocity(WORLD_BOUNDS);
		playerShip.move();

		playerShip.rotateIfTriggered();

		//projectile calls
		playerProjectileManager.collectProjectile(playerShip);
		enemyProjectileManager.collectProjectile(enemyShipsManager);

		playerProjectileManager.updateProjectiles(WORLD_BOUNDS);
		enemyProjectileManager.updateProjectiles(WORLD_BOUNDS);
		enemyShipsManager.updateShips(WORLD_BOUNDS, clock);

		playerProjectileManager.detectCollision(enemyShipsManager);
		enemyProjectileManager.detectCollision(playerShip.getGlobalBounds());

		// Update the window, 
		window.clear();
		window.draw(playerProjectileManager);
		window.draw(enemyProjectileManager);
		window.draw(playerShip);
		window.draw(enemyShipsManager);
		window.display();

	}
	return EXIT_SUCCESS;

}

