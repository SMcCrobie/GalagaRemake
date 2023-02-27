#pragma once
#include "Ship.h"


enum State {
	State0,
	State1,
	State2,
	State3,
	State4,
	State5,


	InvalidState
};

enum Input {
	Hold,
	Input1,
	Input2,
	Input3,
	Input4,
	Input5,


	InvalidInput
};

class KeyboardController
{
public:
	KeyboardController();
	bool PollEventsAndUpdateShipState(sf::Window& window, Ship& ship);

private:
	std::map<sf::Keyboard::Key, ShipControl> m_keyboardToShipControlMap;

};

class StateMachineController
{
public:
	StateMachineController();
	void updateControllerStateAndShipState(const sf::Clock& clocks, Ship& ship);
	void invertMovementInputs();
private:
	State m_currentState;
	State m_previousState;
	int m_deltaBeforeStateChange;//milliseconds
	int m_timeOfLastStateChange;
	int m_totalInputs;
	std::map<State, std::vector<ShipControl>> m_stateToShipControlInputsMap;
	//should change states to be structs, that search them selves for the input provided
	std::map<State, std::map<Input,State>> m_stateWithInputToStateMap;
};

