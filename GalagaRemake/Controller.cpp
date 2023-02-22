#include "Controller.h"
bool IS_PAUSED = false;

KeyboardController::KeyboardController()
{
	m_keyboardToShipControlMap = {
		{ sf::Keyboard::W, MoveUp},
		{ sf::Keyboard::S, MoveDown },
		{ sf::Keyboard::A, MoveLeft },
		{ sf::Keyboard::D, MoveRight },
		{ sf::Keyboard::Enter, FireWeapon1 },
		{ sf::Keyboard::Space, Rotate }
	};
}


bool KeyboardController::PollEventsAndUpdateShipState(sf::Window& window, Ship& ship)
{
	sf::Event event;

	//Poll all game inputs and map
	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) {
			window.close();
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
			return true;
		}

		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.key.code == sf::Mouse::Left) {
				ship.m_shipControlsStateMappings[FireWeapon1] = true;
			}
		}

		//continue if key isn't mapped
		if (m_keyboardToShipControlMap.find(event.key.code) == m_keyboardToShipControlMap.end())
			continue;

		// KEY PRESSED, Runs oen Key input at a time
		if (event.type == sf::Event::KeyPressed) {
			ship.m_shipControlsStateMappings[m_keyboardToShipControlMap.at(event.key.code)] = true;
		
		}
		if (event.type == sf::Event::KeyReleased) {
			if (m_keyboardToShipControlMap.at(event.key.code) > MoveRight)
				continue;

			//state should be turned off, on key release
			ship.m_shipControlsStateMappings[m_keyboardToShipControlMap.at(event.key.code)] = false;
		}

	}

	return false;
}

StateMachineController::StateMachineController() :
	m_currentState(State0),
	m_previousState(InvalidState),
	m_totalInputs(4),
	m_deltaBeforeStateChange(200),
	m_timeOfLastStateChange(0),
	m_stateToShipControlInputsMap{
		{State0, std::vector{ MoveUp, FireWeapon1}},
		{State1, std::vector{ MoveLeft }},
		{State2, std::vector{ MoveRight }},
		{State3, std::vector{ FireWeapon1 }} },
	m_stateWithInputToStateMap{
		{State0, {
			{Hold,		State0},
			{Input1,	State1},
			{Input2,	State2}}
		},
		{State1, {
			{Hold,		State1},
			{Input1,	State0}}
		},
		{State2, {
			{Hold,		State2},
			{Input1,	State0}} 
		},
		{State3, {
			{Hold,		State3},
			{Input1,	State0}}
		}
	}
{
}

void StateMachineController::updateControllerStateAndShipState(const sf::Clock& clock, Ship& ship)
{
	auto currentTime = clock.getElapsedTime().asMilliseconds();
	if (currentTime - m_timeOfLastStateChange < m_deltaBeforeStateChange)
		return;

	m_timeOfLastStateChange = currentTime;
	//get a random input, might be bad since the loop runs every 25 milliseconds
	Input newInput = Input(rand() % m_totalInputs);

	//update state
	auto newState = m_stateWithInputToStateMap.at(m_currentState).find(newInput);
	if (newState != m_stateWithInputToStateMap.at(m_currentState).end())
		m_currentState = newState->second;

	//Can't do this currently because the ship currently shuts its own fire1 off
	//if (m_currentState == m_previousState)
	//	return;


	//update ship inputs based on new state
	std::vector<ShipControl>& currentShipControlVec = m_stateToShipControlInputsMap.at(m_currentState);

	for (auto it = ship.m_shipControlsStateMappings.begin(); it != ship.m_shipControlsStateMappings.end(); it++) {
		it->second = false;
	}
	for (auto it = currentShipControlVec.begin(); it != currentShipControlVec.end(); it++) {
		ship.m_shipControlsStateMappings[*it] = true;
	}
	return;
}
