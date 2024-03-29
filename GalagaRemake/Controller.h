#pragma once
#include "PlayerShip.h"


enum State {
	State0,
	State1,
	State2,
	State3,
	State4,
	State5,
	State6,


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
	void initKeyMappings();
	KeyboardController();

	void flipHorizontalControls();
	void flipVerticalControls();
	void swapControlsAndStatesBasedOnMovementSetting(Ship& ship);
	void PollEventsAndUpdateShipState(sf::Window& window, PlayerShip& ship);

private:
	std::map<sf::Keyboard::Key, ShipControl> m_keyboardToShipControlMap;

};

class StateMachineController
{
public:
	StateMachineController();
	StateMachineController(std::map<State, std::vector<ShipControl>> stateToShipControlInputsMap,
	                       std::map<State, std::map<Input, State>> stateWithInputToStateMap,
	                       int totalInputs,
	                       int deltaBeforeStateChange = 200);
	bool isItTimeToUpdateState(const sf::Int32& currentTime) const;
	void updateControllerStateAndShipState(Ship& ship);
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

