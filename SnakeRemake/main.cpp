
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

const int BASE_SPEED = 480;//amount of counters triggered before move
//const sf::Vector2f ORIGINAL_DIRECTION(0, 30);

class Snake_Block : public sf::RectangleShape {
public:
	sf::Vector2f direction;
	sf::Vector2f PastPosition;
	Snake_Block(sf::Vector2f origin) : RectangleShape(origin) {}

};

void reverse(list<Snake_Block>* snake);
void intializeSnake(list<Snake_Block>* pSnake, int xStartPos);
bool intialize();

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

	bool pause = false;
	bool game_over = false;
	bool moveLeft = false;
	bool moveRight = false;
	bool moveUp = false;
	bool moveDown = false;

	sf::RectangleShape ship(sf::Vector2f(20, 20));
	ship.setPosition(sf::Vector2f(300, 800));
	ship.setFillColor(sf::Color::Blue);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event)) {
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();

			// KEY PRESSED IF
			if (event.type == sf::Event::KeyPressed) {
				// IF ESCAPE
				//    movCommand = true;
				switch (event.key.code) {
					//ESCAPE KEY PRESSED
					case sf::Keyboard::Escape:
						window.close();
						break;

					case sf::Keyboard::Left:
						//direction = sf::Vector2f(-MOVE_INCREMENT, 0);
						moveLeft = true;
						break;
					case sf::Keyboard::Up:
						//direction = sf::Vector2f(0, -MOVE_INCREMENT);
						moveUp = true;
						break;
					case sf::Keyboard::Right:
						//direction = sf::Vector2f(MOVE_INCREMENT, 0);
						moveRight = true;
						break;
					case sf::Keyboard::Down:
						//direction = sf::Vector2f(0, MOVE_INCREMENT);
						moveDown = true;
						break;
					case sf::Keyboard::P:
						if (!pause)
							pause = true;
						else
							pause = false;
						break;
					case sf::Keyboard::Space:
						if (!pause)
							pause = true;
						else
							pause = false;
						break;
				}
			}

			if (event.type == sf::Event::KeyReleased) {
				// IF ESCAPE
				//    movCommand = true;
				switch (event.key.code) {


				case sf::Keyboard::Left:
					//direction = sf::Vector2f(-MOVE_INCREMENT, 0);
					moveLeft = false;
					break;
				case sf::Keyboard::Up:
					//direction = sf::Vector2f(0, -MOVE_INCREMENT);
					moveUp = false;
					break;
				case sf::Keyboard::Right:
					//direction = sf::Vector2f(MOVE_INCREMENT, 0);
					moveRight = false;
					break;
				case sf::Keyboard::Down:
					//direction = sf::Vector2f(0, MOVE_INCREMENT);
					moveDown = false;
					break;
				}
			}

		}
	


		//MOVEMENT FOR SNAKE1	
		if (!pause && !game_over && clock.getElapsedTime().asMilliseconds() >= 50) {

			//apply movement to direction, after RESET, opposing directions cancel
			direction = sf::Vector2f(0, 0);
			if (moveUp)
				direction.y += -MOVE_INCREMENT;
			if (moveDown)
				direction.y += MOVE_INCREMENT;
			if (moveLeft)
				direction.x += -MOVE_INCREMENT;
			if (moveRight)
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