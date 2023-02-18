#include "Controller.h"

Controller::Controller()
{
	m_KeyboardToShipControlMappings = {
		{ sf::Keyboard::Up, MoveUp},
		{ sf::Keyboard::Down, MoveDown },
		{ sf::Keyboard::Left, MoveLeft },
		{ sf::Keyboard::Right, MoveRight },
		{ sf::Keyboard::F, FireWeapon1 }
	};
}

void Controller::PollEventsAndUpdateShipState(sf::Window& window, Ship& ship)
{
	sf::Event event;

	//Poll all game inputs and map
	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) {
			window.close();
		}
		//continue if key isn't mapped
		if (m_KeyboardToShipControlMappings.find(event.key.code) == m_KeyboardToShipControlMappings.end())
			continue;

		// KEY PRESSED, Runs oen Key input at a time
		if (event.type == sf::Event::KeyPressed) {
			if (m_KeyboardToShipControlMappings.at(event.key.code) <= FireWeapon2) {
				//state should be set turned on, on key press
				ship.m_shipControlsStateMappings[m_KeyboardToShipControlMappings.at(event.key.code)] = true;
			}
			else {
				//state should be inverted on key press
				ship.m_shipControlsStateMappings[m_KeyboardToShipControlMappings.at(event.key.code)] = !ship.m_shipControlsStateMappings[m_KeyboardToShipControlMappings.at(event.key.code)];
			}
		}
		if (event.type == sf::Event::KeyReleased) {
			if (m_KeyboardToShipControlMappings.at(event.key.code) > MoveRight)
				continue;

			//state should be turned off, on key release
			ship.m_shipControlsStateMappings[m_KeyboardToShipControlMappings.at(event.key.code)] = false;
		}

	}

	return;
}
