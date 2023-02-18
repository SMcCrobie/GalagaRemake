
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
#include "projectile.h"
#include "BoundedFloatRect.h"
#include "Ship.h"
#include "Controller.h"
#include "main.h"

using namespace std;

//Global Variables
BoundedFloatRect WORLD_BOUNDS(0.0f, 0.0f, 600.0f, 1000.0f);
sf::View WORLD_VIEW(WORLD_BOUNDS);

template <typename T,
	class = typename enable_if <is_base_of <sf::Drawable, T>::value, bool>::type>
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
		cout << "failed to load sprite" << endl;
		return EXIT_FAILURE;
	}

	//intialize ship
	Ship playerShip;
	playerShip.setTexture(shipAnimations);
	playerShip.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(45, 48)));
	playerShip.setPosition(sf::Vector2f(300, 800));
	playerShip.setColor(sf::Color(255, 255, 255, 200));


	//intialize controller
	Controller playerController;


	//counters
	sf::Int32 timeOfLastGameLoop = 0;
	sf::Int32 timeOfLastEnemyShip = 0;




	//containers for drawables
	list<Projectile> projectiles;
	list<Ship> enemyShips;
	
	


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


		//Fire, MOVE SHIP FIRST
		if (playerShip.isFire1) {
			playerShip.isFire1 = false;
			sf::FloatRect currentShipPosition = playerShip.getGlobalBounds();
			Projectile tempProjectile(sf::Vector2f(3, 12));
			tempProjectile.setFillColor(sf::Color(0x05ecf1ff));
			tempProjectile.setPosition(currentShipPosition.left + (currentShipPosition.width / 2) - 2, currentShipPosition.top);
			projectiles.push_back(tempProjectile);
		}

		for (list<Projectile>::iterator it = projectiles.begin(); it != projectiles.end(); it++)
		{
			it->move();
			sf::FloatRect projectileBounds = it->getGlobalBounds();
			if (!WORLD_BOUNDS.intersects(projectileBounds)) {
				it = projectiles.erase(it);
				if (it == projectiles.end())
					break;
			}
		}
	
		// Update the window, use template function for any list of drawable subclass
		window.clear();
		DrawList(projectiles, window);
		DrawList(enemyShips, window);
		window.draw(playerShip);
		window.display();

	}
	return EXIT_SUCCESS;

}

