#pragma once
#include "Ship.h"


class KeyboardController
{
public:
	class KeyboardController();
	void PollEventsAndUpdateShipState(sf::Window& window, Ship& ship);

private:
	std::map<sf::Keyboard::Key, ShipControl> m_KeyboardToShipControlMappings;

};

