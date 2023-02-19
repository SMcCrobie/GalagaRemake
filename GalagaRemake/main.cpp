
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

//Global Variables
BoundedFloatRect WORLD_BOUNDS(0.0f, 0.0f, 600.0f, 1000.0f);
sf::View WORLD_VIEW(WORLD_BOUNDS);

template <typename T,
	class = typename std::enable_if <std::is_base_of <sf::Drawable, T>::value, bool>::type>
void DrawList(std::list<T>& list, sf::RenderWindow& window) {
	for (auto it = list.begin(); it != list.end(); it++) {
		window.draw(*it);
	}
	return;
}



int main(int, char const**)
{
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
	playerShip.setPosition(sf::Vector2f(300, 800));
	//playerShip.setColor(sf::Color::Magenta);


	//intialize controller
	KeyboardController playerController;


	//counters
	sf::Int32 timeOfLastGameLoop = 0;
	sf::Int32 timeOfLastEnemyShip = 0;



	//containers for drawables
	ProjectileManager projectileManager;
	std::list<Ship> enemyShips;
	
	


	while (window.isOpen())
	{
		if (clock.getElapsedTime().asMilliseconds() - timeOfLastGameLoop <= 25) {
			continue;
		}
		timeOfLastGameLoop = clock.getElapsedTime().asMilliseconds();



		//increment the world
		WORLD_VIEW.move(0.f, WORLD_VIEW_MOVEMENT);
		WORLD_BOUNDS.top += WORLD_VIEW_MOVEMENT;
		WORLD_BOUNDS.bottom += WORLD_VIEW_MOVEMENT;
		window.setView(WORLD_VIEW);

		//Poll for events
		playerController.PollEventsAndUpdateShipState(window, playerShip);


		//apply texture, based on events from player controller
		playerShip.setTextureRectBasedOnShipState();


		//Apply inputs and envirmental factors to movement
		playerShip.updateShipVelocity(WORLD_BOUNDS);
		playerShip.move();

		//reduced projectiles to two function calls, 
		projectileManager.collectProjectile(playerShip);
		projectileManager.updateProjectiles(WORLD_BOUNDS);

	
		// Update the window, use template function for any list of drawable subclass
		window.clear();
		window.draw(projectileManager);
		//DrawList(enemyShips, window);
		window.draw(playerShip);
		window.display();

	}
	return EXIT_SUCCESS;

}

