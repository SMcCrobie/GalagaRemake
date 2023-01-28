
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

static float MOVE_INCREMENT = 5;

int main(int, char const**)
{
	srand(time(NULL));

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(600, 1000), "Galaga!");
	window.setFramerateLimit(120);

	//DIRECTION AND COUNTER VARIABLES
	int counter = 0;
	sf::Vector2f direction(0, 0);

	//Clock
	sf::Clock clock;
	clock.restart();
	int frame = 0;

	map<GameInput, bool> GameInputStateMappings;
	const map<sf::Keyboard::Key, GameInput> GameKeyToInputMappings = {
		{sf::Keyboard::W, MoveUp},
		{sf::Keyboard::S, MoveDown},
		{sf::Keyboard::A, MoveLeft},
		{sf::Keyboard::D, MoveRight},
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
					//state should be turned off, on key press
					GameInputStateMappings[GameKeyToInputMappings.at(event.key.code)] = false;
				}
			}
		}


		//MOVEMENT FOR SNAKE1	
		if (!GameInputStateMappings[PauseGame] && clock.getElapsedTime().asMilliseconds() >= 50) {

			//apply movement to direction, after RESET, opposing directions cancel
			direction = sf::Vector2f(0, 0);
			if (GameInputStateMappings[MoveUp])
				direction.y += -MOVE_INCREMENT;
			if (GameInputStateMappings[MoveDown])
				direction.y += MOVE_INCREMENT;
			if (GameInputStateMappings[MoveLeft])
				direction.x += -MOVE_INCREMENT;
			if (GameInputStateMappings[MoveRight])
				direction.x += MOVE_INCREMENT;
			ship.move(direction);
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