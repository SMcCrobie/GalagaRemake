
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


#define Score_Value_As_Int (clock.getElapsedTime().asMilliseconds()/400 + (killCounter * 100))


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

	sf::Font font;
	if (!font.loadFromFile("Deutsch.ttf"))
	{
		std::cout << "failed to load Deutsch.ttf" << std::endl;
		return EXIT_FAILURE;
	}
	sf::Text score("Score:0" , font);
	score.setPosition(sf::Vector2f(10, 0));
	score.setScale(.65f, .65f);

	sf::Text lifeCounter("Lives: ", font);
	lifeCounter.setPosition(sf::Vector2f(10, 970));
	lifeCounter.setScale(.65f, .65f);

	//intialize ships
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
	enemyProjectile.setVelocity(sf::Vector2f(0, -8));
	enemyShip.setProjectile(enemyProjectile);
	//enemyShip.setScale(1.4f, .6f);
	enemyShip.rotate180();
	
	//intialize controller
	KeyboardController playerController;
	StateMachineController enemyController;
	
	
	
	std::vector<Ship> lives;
	Ship lifeSymbol = playerShip;
	lifeSymbol.setPosition(70, 970);
	lifeSymbol.setScale(.4f, .4f);
	lifeSymbol.rotate(30.f);
	lifeSymbol.setVelocity(0, WORLD_VIEW_MOVEMENT);
	Ship lifeSymbol2 = lifeSymbol;
	lifeSymbol2.setPosition(95, 970);
	Ship lifeSymbol3 = lifeSymbol;
	lifeSymbol3.setPosition(120, 970);


	//counters
	int timeOfLastGameLoop = 0;
	int timeOfLastEnemyShip = -1000;
	int deltaTillNextEnemyShip = 6000;
	int killCounter = 0;

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
		if (clock.getElapsedTime().asMilliseconds() - timeOfLastEnemyShip >= deltaTillNextEnemyShip) {
			float shipWidth = enemyShip.getGlobalBounds().width;//somehow this is wrong

			float xCoordinate = 56.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (589.f - 56.f)));
			enemyShip.setPosition(sf::Vector2f(xCoordinate, WORLD_BOUNDS.top - 50.f));
			enemyShipsManager.createShip(enemyShip);
			
		
			//enemyship Upgrade
			if (Score_Value_As_Int > 2000) {
				float xCoordinate = 56.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (589.f - 56.f)));
				enemyShip.setPosition(sf::Vector2f(xCoordinate, WORLD_BOUNDS.bottom + 50.f));
				enemyShip.rotate180();
				enemyShipsManager.createShip(enemyShip);
				enemyShip.rotate180();
			}

			timeOfLastEnemyShip = clock.getElapsedTime().asMilliseconds();
			deltaTillNextEnemyShip = deltaTillNextEnemyShip - 40;
		}
	
	
		score.setString("Score: " + std::to_string(Score_Value_As_Int));



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

		playerProjectileManager.detectCollision(enemyShipsManager, killCounter);
		enemyProjectileManager.detectCollision(playerShip.getGlobalBounds());

		// Update the window, 
		window.clear();
		window.draw(playerProjectileManager);
		window.draw(enemyProjectileManager);
		window.draw(playerShip);
		window.draw(enemyShipsManager);
		
		
		
		window.draw(score);
		window.draw(lifeCounter);
		window.draw(lifeSymbol);
		window.draw(lifeSymbol2);
		window.draw(lifeSymbol3);
		
		
		
		window.display();

	}
	return EXIT_SUCCESS;

}

