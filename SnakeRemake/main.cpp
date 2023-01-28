
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <algorithm>
#include <map>

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

static float SINGLE_THRUST_DIRECTION_INCREMENT = 1.5;
static float DUAL_THRUST_DIRECTION_INCREMENT = SINGLE_THRUST_DIRECTION_INCREMENT * .66;
static float RESISTENCE_INCREMENT = SINGLE_THRUST_DIRECTION_INCREMENT / 4;
static float MAX_HORIZONTAL_SPEED = 12;
static float MAX_VERTICAL_SPEED = 8;

int main(int, char const**)
{
	srand(time(NULL));

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(600, 1000), "Galaga!");
	window.setFramerateLimit(120);

	//DIRECTION AND COUNTER VARIABLES
	int counter = 0;
	sf::Vector2f shipVelocity(0, 0);

	//Clock
	sf::Clock clock;
	clock.restart();
	int frame = 0;

	map<GameInput, bool> GameInputStateMappings;
	const map<sf::Keyboard::Key, GameInput> GameKeyToInputMappings = {
		{sf::Keyboard::Up, MoveUp},
		{sf::Keyboard::Down, MoveDown},
		{sf::Keyboard::Left, MoveLeft},
		{sf::Keyboard::Right, MoveRight},
		{sf::Keyboard::Space, PauseGame},
		{sf::Keyboard::Escape, QuitGame}
	};

	for (int i = MoveUp; i < InvalidInput; i++) {
		GameInputStateMappings[GameInput(i)] = false;
	}

	sf::CircleShape ship(20, 3);
	ship.setPosition(sf::Vector2f(300, 800));
	ship.setFillColor(sf::Color::Blue);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (GameKeyToInputMappings.find(event.key.code) != GameKeyToInputMappings.end()) {
				// KEY PRESSED, Runs oen Key input at a time
				if (event.type == sf::Event::KeyPressed) {
					if (GameKeyToInputMappings.at(event.key.code) <= MoveRight) {
						//state should be set turned on, on key press
						GameInputStateMappings[GameKeyToInputMappings.at(event.key.code)] = true;
					}
					else {
						//state should be inverted on key press
						GameInputStateMappings[GameKeyToInputMappings.at(event.key.code)] = !GameInputStateMappings[GameKeyToInputMappings.at(event.key.code)];
					}
				}
				if (event.type == sf::Event::KeyReleased) {
					//state should be turned off, on key release
					GameInputStateMappings[GameKeyToInputMappings.at(event.key.code)] = false;
				}
			}
		}


		//MOVEMENT FOR SHIP	
		if (!GameInputStateMappings[PauseGame] && clock.getElapsedTime().asMilliseconds() >= 50) {
			
			if(shipVelocity.y > 0 )
				shipVelocity.y += -RESISTENCE_INCREMENT;
			if (shipVelocity.y < 0)
				shipVelocity.y += RESISTENCE_INCREMENT;
			
			if(shipVelocity.x > 0)
				shipVelocity.x += -RESISTENCE_INCREMENT;
			if (shipVelocity.x < 0)
				shipVelocity.x += RESISTENCE_INCREMENT;

			//apply movement to shipVelocity, after State update, opposing shipVelocitys cancel
			if (GameInputStateMappings[MoveUp] && shipVelocity.y >= -MAX_VERTICAL_SPEED)
				shipVelocity.y += GameInputStateMappings[MoveLeft] || GameInputStateMappings[MoveRight] ? -DUAL_THRUST_DIRECTION_INCREMENT : -SINGLE_THRUST_DIRECTION_INCREMENT;
			if (GameInputStateMappings[MoveDown] && shipVelocity.y <= MAX_VERTICAL_SPEED)
				shipVelocity.y += GameInputStateMappings[MoveLeft] || GameInputStateMappings[MoveRight] ? DUAL_THRUST_DIRECTION_INCREMENT : SINGLE_THRUST_DIRECTION_INCREMENT;



			if (GameInputStateMappings[MoveLeft] && shipVelocity.x >= -MAX_HORIZONTAL_SPEED)
				shipVelocity.x += GameInputStateMappings[MoveUp] || GameInputStateMappings[MoveDown] ? -DUAL_THRUST_DIRECTION_INCREMENT : -SINGLE_THRUST_DIRECTION_INCREMENT;
			if (GameInputStateMappings[MoveRight] && shipVelocity.x <= MAX_HORIZONTAL_SPEED)
				shipVelocity.x += GameInputStateMappings[MoveUp] || GameInputStateMappings[MoveDown] ? DUAL_THRUST_DIRECTION_INCREMENT : SINGLE_THRUST_DIRECTION_INCREMENT;
			ship.move(shipVelocity);
			clock.restart();

		}
		window.clear();
		window.draw(ship);

		// Update the window
		window.display();
		frame++;
	}
	return EXIT_SUCCESS;

}