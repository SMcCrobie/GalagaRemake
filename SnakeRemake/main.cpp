
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



using namespace std;

enum GameInput {
	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight,
	FireWeapon1,
	FireWeapon2,
	PauseGame,
	QuitGame,

	InvalidInput
};

void PollEventFromInputs(sf::Window& window, map<GameInput, bool>& GameInputStateMappings, const map<sf::Keyboard::Key, GameInput>& GameKeyToInputMappings);
void UpdateShipVelocity(sf::Vector2u windowSize, sf::FloatRect shipBounds, sf::Vector2f& shipVelocity, const map<GameInput, bool>& GameInputStateMappings);


//Global Variables
static float SINGLE_THRUST_DIRECTION_INCREMENT = .2;
static float HORIZONTAL_DIRECTION_INCREMENT = .45;
static float RESISTENCE_MULTIPLIER = .96;
static float MAX_HORIZONTAL_SPEED = 8;
static float MAX_VERTICAL_SPEED = 4;
static float WORLD_BOUNDS_MARGIN = 10;


int main(int, char const**)
{
	srand(time(NULL));

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(600, 1000), "Galaga!");
	window.setKeyRepeatEnabled(false);
	//window.setFramerateLimit(120);

	//DIRECTION AND COUNTER VARIABLES
	int counter = 0;
	sf::Vector2f shipVelocity(0, 0);

	//Clock
	sf::Clock clock;
	clock.restart();

	map<GameInput, bool> GameInputStateMappings;
	const map<sf::Keyboard::Key, GameInput> GameKeyToInputMappings = {
		{sf::Keyboard::Up, MoveUp},
		{sf::Keyboard::Down, MoveDown},
		{sf::Keyboard::Left, MoveLeft},
		{sf::Keyboard::Right, MoveRight},
		{sf::Keyboard::F, FireWeapon1},
		{sf::Keyboard::Space, PauseGame},
		{sf::Keyboard::Escape, QuitGame}
	};

	for (int i = MoveUp; i < InvalidInput; i++) {
		GameInputStateMappings[GameInput(i)] = false;
	}

	sf::Texture shipAnimations;
	if (!shipAnimations.loadFromFile("C:\\Users\\Sean McCrobie\\source\\repos\\GalageRemake\\x64\\Release\\ShipAnimations.png"))
	{
		cout << "failed to load sprite" << endl;
		return EXIT_FAILURE;
	}

	sf::Vector2i shipAnimationFrame(45,48);

	
	sf::Sprite ship(shipAnimations, sf::IntRect(sf::Vector2i(0,0),shipAnimationFrame));
	ship.setPosition(sf::Vector2f(300, 800));
	ship.setColor(sf::Color(255, 255, 255, 200));


	//projectiles fired
	list<Projectile> projectiles;

	while (window.isOpen())
	{
		//Poll for evetn
		PollEventFromInputs(window, GameInputStateMappings, GameKeyToInputMappings);
		if (GameInputStateMappings[PauseGame] || clock.getElapsedTime().asMilliseconds() <= 25) {
			// Update the window
			window.clear();
			window.draw(ship);
			for (Projectile projectile : projectiles)
			{
				window.draw(projectile);
			}
			window.display();
			continue;
		}

		//apply texture base on input
		if (GameInputStateMappings.at(MoveLeft) && !GameInputStateMappings.at(MoveRight)) {
			if (GameInputStateMappings.at(MoveUp) && !GameInputStateMappings.at(MoveDown)) {
				ship.setTextureRect(sf::IntRect(sf::Vector2i(shipAnimationFrame.x*2, shipAnimationFrame.y), shipAnimationFrame));
			}
			else if (!GameInputStateMappings.at(MoveUp) && GameInputStateMappings.at(MoveDown)) {
				ship.setTextureRect(sf::IntRect(sf::Vector2i(0, shipAnimationFrame.y), shipAnimationFrame));
			}
			else {
				ship.setTextureRect(sf::IntRect(sf::Vector2i(shipAnimationFrame.x, shipAnimationFrame.y), shipAnimationFrame));
			}
		}
		else if (!GameInputStateMappings.at(MoveLeft) && GameInputStateMappings.at(MoveRight)) {
			if (GameInputStateMappings.at(MoveUp) && !GameInputStateMappings.at(MoveDown)) {
				ship.setTextureRect(sf::IntRect(sf::Vector2i(shipAnimationFrame.x * 2, shipAnimationFrame.y*2), shipAnimationFrame));
			}
			else if (!GameInputStateMappings.at(MoveUp) && GameInputStateMappings.at(MoveDown)) {
				ship.setTextureRect(sf::IntRect(sf::Vector2i(0, shipAnimationFrame.y*2), shipAnimationFrame));
			}
			else {
				ship.setTextureRect(sf::IntRect(sf::Vector2i(shipAnimationFrame.x , shipAnimationFrame.y*2), shipAnimationFrame));
			}
		}
		else {
			if (GameInputStateMappings.at(MoveUp) && !GameInputStateMappings.at(MoveDown)) {
				ship.setTextureRect(sf::IntRect(sf::Vector2i(shipAnimationFrame.x * 2, 0), shipAnimationFrame));
			}
			else if (!GameInputStateMappings.at(MoveUp) && GameInputStateMappings.at(MoveDown)) {
				ship.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), shipAnimationFrame));
			}
			else {
				ship.setTextureRect(sf::IntRect(sf::Vector2i(shipAnimationFrame.x, 0), shipAnimationFrame));
			}
		}



		//Apply inputs and envirmental factors to movement
		UpdateShipVelocity(window.getSize(), ship.getGlobalBounds(), shipVelocity, GameInputStateMappings);
		ship.move(shipVelocity);
		clock.restart();

		//Fire, MOVE SHIP FIRST
		if (GameInputStateMappings[FireWeapon1]) {
			GameInputStateMappings[FireWeapon1] = false;
			sf::FloatRect currentShipPosition = ship.getGlobalBounds();
			Projectile tempProjectile(sf::Vector2f(3, 12));
			tempProjectile.setFillColor(sf::Color(0x05ecf1ff));
			tempProjectile.setPosition(currentShipPosition.left + (currentShipPosition.width / 2) - 2, currentShipPosition.top);
			projectiles.push_back(tempProjectile);
		}

		for (list<Projectile>::iterator it = projectiles.begin(); it != projectiles.end(); it++)
		{
			it->moveProjectile();
			sf::FloatRect projectileBounds = it->getGlobalBounds();
			if (projectileBounds.top + projectileBounds.height < -10) {
				it = projectiles.erase(it);
				if (it == projectiles.end())
					break;
			}
		}
	

		// Update the window
		window.clear();
		window.draw(ship);
		for (Projectile projectile : projectiles)
		{
			window.draw(projectile);
		}
		window.display();
	
	}
	return EXIT_SUCCESS;

}

void PollEventFromInputs(sf::Window& window, map<GameInput, bool>& GameInputStateMappings, const map<sf::Keyboard::Key, GameInput>& GameKeyToInputMappings) {
	sf::Event event;

	//Poll all game inputs and map
	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) {
			window.close();
		}
		//continue if key isn't mapped
		if (GameKeyToInputMappings.find(event.key.code) == GameKeyToInputMappings.end())
			continue;

		// KEY PRESSED, Runs oen Key input at a time
		if (event.type == sf::Event::KeyPressed) {
			if (GameKeyToInputMappings.at(event.key.code) <= FireWeapon2) {
				//state should be set turned on, on key press
				GameInputStateMappings[GameKeyToInputMappings.at(event.key.code)] = true;
			}
			else {
				//state should be inverted on key press
				GameInputStateMappings[GameKeyToInputMappings.at(event.key.code)] = !GameInputStateMappings[GameKeyToInputMappings.at(event.key.code)];
			}
		}
		if (event.type == sf::Event::KeyReleased) {
			if (GameKeyToInputMappings.at(event.key.code) > MoveRight)
				continue;

			//state should be turned off, on key release
			GameInputStateMappings[GameKeyToInputMappings.at(event.key.code)] = false;
		}

	}
	return;

}
void UpdateShipVelocity(sf::Vector2u windowSize, sf::FloatRect shipBounds, sf::Vector2f& shipVelocity, const map<GameInput, bool>& GameInputStateMappings)
{
	//apply air resistence
	if (shipVelocity.y != 0)
		shipVelocity.y = shipVelocity.y * RESISTENCE_MULTIPLIER;

	if (shipVelocity.x != 0)
		shipVelocity.x = shipVelocity.x * RESISTENCE_MULTIPLIER;

	//apply thrust to shipVelocity, after State update, opposing Thrusts cancel
	if (GameInputStateMappings.at(MoveUp) && shipVelocity.y >= -MAX_VERTICAL_SPEED)
		shipVelocity.y += -SINGLE_THRUST_DIRECTION_INCREMENT;
	if (GameInputStateMappings.at(MoveDown) && shipVelocity.y <= MAX_VERTICAL_SPEED)
		shipVelocity.y += SINGLE_THRUST_DIRECTION_INCREMENT;



	if (GameInputStateMappings.at(MoveLeft) && shipVelocity.x >= -MAX_HORIZONTAL_SPEED)
		shipVelocity.x += -HORIZONTAL_DIRECTION_INCREMENT;
	if (GameInputStateMappings.at(MoveRight) && shipVelocity.x <= MAX_HORIZONTAL_SPEED)
		shipVelocity.x += HORIZONTAL_DIRECTION_INCREMENT;


	//WORLD BOUNDS, these are better but stil prob not best
	while (shipBounds.left + shipBounds.width + shipVelocity.x > windowSize.x - WORLD_BOUNDS_MARGIN
		|| shipBounds.left + shipVelocity.x < WORLD_BOUNDS_MARGIN) {
		if (abs(shipBounds.left + shipBounds.width - windowSize.x - WORLD_BOUNDS_MARGIN) < .01f
			|| (abs(shipBounds.left - WORLD_BOUNDS_MARGIN) < .01f)) {
			shipVelocity.x = 0;
			break;
		}
		shipVelocity.x = shipVelocity.x * .5f;
	}
	while (shipBounds.top + shipBounds.height + shipVelocity.y > windowSize.y - WORLD_BOUNDS_MARGIN
		|| shipBounds.top + shipVelocity.y < WORLD_BOUNDS_MARGIN) {
		if (abs(shipBounds.top + shipBounds.height - windowSize.y - WORLD_BOUNDS_MARGIN) < .01f
			|| (abs(shipBounds.top - WORLD_BOUNDS_MARGIN) < .01f)) {
			shipVelocity.y = 0;
			break;
		}
		shipVelocity.y = shipVelocity.y * .5f;
	}

	return;
}
