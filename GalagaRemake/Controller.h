#pragma once
#include "Ship.h"


class Controller
{
public:
	Controller();
	void PollEventsAndUpdateShipState(sf::Window& window, Ship& ship);

private:
	std::map<sf::Keyboard::Key, ShipControl> m_KeyboardToShipControlMappings;

};

