
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
void UpdateShipVelocity(sf::Vector2u windowSize, BoundedFloatRect shipBounds, sf::Vector2f& shipVelocity, const map<GameInput, bool>& GameInputStateMappings);


//Global Variables
const float SINGLE_THRUST_DIRECTION_INCREMENT = .3;
const float HORIZONTAL_DIRECTION_INCREMENT = .55;
const float RESISTENCE_MULTIPLIER = .96;
float const MAX_HORIZONTAL_SPEED = 8;
const float MAX_VERTICAL_SPEED = 6;
const float WORLD_BOUNDS_MARGIN = 10;
const float WORLD_VIEW_MOVEMENT = -1.25f;
BoundedFloatRect WORLD_BOUNDS(0.0f, 0.0f, 600.0f, 1000.0f);
sf::View WORLD_VIEW(WORLD_BOUNDS);
//replace everything you can with this
//Can use itersects via this

int main(int, char const**)
{
	srand(time(NULL));


	// Create the main window
	// X goes right and Y goes down
	// Y is inverted 0 at the top 1000 at the bottom
	sf::RenderWindow window(sf::VideoMode(600, 1000), "Galaga!");
	window.setKeyRepeatEnabled(false);
	window.setView(WORLD_VIEW);
	
	
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
	if (!shipAnimations.loadFromFile("ShipAnimations.png"))
	{
		cout << "failed to load sprite" << endl;
		return EXIT_FAILURE;
	}
	//
	sf::Vector2i shipAnimationFrame(45,48);

	
	sf::Sprite ship(shipAnimations, sf::IntRect(sf::Vector2i(0,0),shipAnimationFrame));
	ship.setPosition(sf::Vector2f(300, 800));
	ship.setColor(sf::Color(255, 255, 255, 200));

	sf::RectangleShape background(sf::Vector2f(600, 1000));
	shipAnimations.setRepeated(true);
	background.setTexture(&shipAnimations);
	





	//projectiles fired
	list<Projectile> projectiles;

	while (window.isOpen())
	{
		if (GameInputStateMappings[PauseGame] || clock.getElapsedTime().asMilliseconds() <= 25) {
			continue;
		}

		//increment the world
		WORLD_VIEW.move(0.f, WORLD_VIEW_MOVEMENT);
		WORLD_BOUNDS.top += WORLD_VIEW_MOVEMENT;
		WORLD_BOUNDS.bottom += WORLD_VIEW_MOVEMENT;
		window.setView(WORLD_VIEW);

		//Poll for evetn
		PollEventFromInputs(window, GameInputStateMappings, GameKeyToInputMappings);

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
			if (!WORLD_BOUNDS.intersects(projectileBounds)) {
				it = projectiles.erase(it);
				if (it == projectiles.end())
					break;
			}
		}
	

		// Update the window
		window.clear();
		//window.draw(background);
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

/// <summary>
/// Needs to be abstracted away it to class object functions, with global instance variables to share
/// </summary>
void UpdateShipVelocity(sf::Vector2u windowSize, BoundedFloatRect shipBounds, sf::Vector2f& shipVelocity, const map<GameInput, bool>& GameInputStateMappings)
{
	//apply resistence
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



	//WORLD BOUNDS, these are better but stil prob not best. Maybe derive sf::FloatRec and add bounds so you dont have to add the top and height or the left and width to get the right coordinate
	while (shipBounds.right + shipVelocity.x > WORLD_BOUNDS.right - WORLD_BOUNDS_MARGIN
		|| shipBounds.left + shipVelocity.x < WORLD_BOUNDS.left + WORLD_BOUNDS_MARGIN) {
		
		
		
		if (abs(shipBounds.right - WORLD_BOUNDS.right - WORLD_BOUNDS_MARGIN) < .01f
			|| (abs(shipBounds.left - WORLD_BOUNDS.left + WORLD_BOUNDS_MARGIN) < .01f )) {
			shipVelocity.x = 0;
			break;
		}
		shipVelocity.x = shipVelocity.x * .5f;



	}
	while (shipBounds.bottom + shipVelocity.y > WORLD_BOUNDS.bottom - WORLD_BOUNDS_MARGIN
		|| shipBounds.top + shipVelocity.y < WORLD_BOUNDS.top + WORLD_BOUNDS_MARGIN) {
		
		
		if (shipBounds.bottom > WORLD_BOUNDS.bottom - WORLD_BOUNDS_MARGIN ) {
			shipVelocity.y = (WORLD_BOUNDS.bottom - WORLD_BOUNDS_MARGIN) - shipBounds.bottom;
			break;
		}
		if(shipBounds.top < WORLD_BOUNDS.top + WORLD_BOUNDS_MARGIN) {
			shipVelocity.y = shipBounds.top - (WORLD_BOUNDS.top + WORLD_BOUNDS_MARGIN);
			break;
		}
		shipVelocity.y = shipVelocity.y * .5f;
	
	}

	return;
}
