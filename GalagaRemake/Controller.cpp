#include "Controller.h"
#include "GameState.h"

void KeyboardController::initKeyMappings()
{
	//Controller A
	m_keyboardToShipControlMap = {
		{ sf::Keyboard::W, MoveUp},
		{ sf::Keyboard::S, MoveDown },
		{ sf::Keyboard::A, MoveLeft },
		{ sf::Keyboard::D, MoveRight },
		{ sf::Keyboard::Enter, FireWeapon1 },
		{ sf::Keyboard::Space, Rotate }
	};

	//Controller B
	//m_keyboardToShipControlMap = {
	//	{ sf::Keyboard::Up, MoveUp},
	//	{ sf::Keyboard::Down, MoveDown },
	//	{ sf::Keyboard::Left, MoveLeft },
	//	{ sf::Keyboard::Right, MoveRight },
	//	{ sf::Keyboard::Space, FireWeapon1 },
	//	{ sf::Keyboard::LShift, Rotate }
	//};
}

KeyboardController::KeyboardController()
{
	initKeyMappings();
}

void KeyboardController::flipHorizontalControls()
{
	sf::Keyboard::Key leftKey, rightKey;
	for(auto& [key, shipControl] : m_keyboardToShipControlMap)
	{
		if (shipControl == MoveLeft)
			leftKey = key;
		if (shipControl == MoveRight)
			rightKey = key;
	}
	m_keyboardToShipControlMap[leftKey] = MoveRight;
	m_keyboardToShipControlMap[rightKey] = MoveLeft;
}

void KeyboardController::flipVerticalControls()
{
	sf::Keyboard::Key upKey, downKey;
	for (auto& [key, shipControl] : m_keyboardToShipControlMap)
	{
		if (shipControl == MoveUp)
			upKey = key;
		if (shipControl == MoveDown)
			downKey = key;
	}
	m_keyboardToShipControlMap[upKey] = MoveDown;
	m_keyboardToShipControlMap[downKey] = MoveUp;
}


void KeyboardController::swapControlsAndStatesBasedOnMovementSetting(Ship& ship)
{
	using namespace GameState;
	switch (movementControlSetting)
	{
	case full_ship_orientation:
		break;
	case window_and_ship_orientation:
		flipHorizontalControls();
		ship.flipHorizontalMovementStates();
		break;
	case full_window_orientation:
		flipHorizontalControls();
		ship.flipHorizontalMovementStates();
		flipVerticalControls();
		ship.flipVerticalMovementStates();
		break;
		
	}
}

void KeyboardController::PollEventsAndUpdateShipState(sf::Window& window, PlayerShip& ship)
{
	sf::Event event{};

	//Poll all game inputs and map
	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed ||
			(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
			window.close();
		}
		if(event.type == sf::Event::LostFocus && !GameState::isMainMenu){
			GameState::isPaused = true;
			continue;
		}
		if (event.type == sf::Event::GainedFocus) {
			GameState::isPaused = false;
			continue;
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab && !GameState::isMainMenu) {
			ship.disableCurrentShipStates();
			GameState::isPaused = !GameState::isPaused;
		}
		if(event.type == sf::Event::Resized && !GameState::ignoreNextResizeEvent && !GameState::isMainMenu){
			ship.disableCurrentShipStates();
			GameState::isPaused = true;
		}
		if(event.type == sf::Event::Resized && GameState::ignoreNextResizeEvent){
			GameState::ignoreNextResizeEvent = false;
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F2) {
			GameState::requiresLevelRestart = true;
			return;
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
			GameState::resetWindow = true;
			return;
		}


		if(GameState::isPaused)
			continue;

		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.key.code == sf::Mouse::Left) {
				ship.m_shipControlsStateMappings[FireWeapon1] = true;
			}
		}
		//continue if not a key press or release
		if (event.type != sf::Event::KeyPressed && event.type != sf::Event::KeyReleased)
			continue;

		//continue if key isn't mapped
		if (m_keyboardToShipControlMap.find(event.key.code) == m_keyboardToShipControlMap.end())
			continue;

		if (!GameState::isKeyTrapActivated && event.key.code != sf::Keyboard::Key::Enter)
		{
			ship.disableCurrentShipStates();
			GameState::isKeyTrapActivated = true;
		}
			

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
}

StateMachineController::StateMachineController() :
	m_currentState(State0),
	m_previousState(InvalidState),
	m_timeOfLastStateChange(0),



	m_totalInputs(4),
	m_deltaBeforeStateChange(200),
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

StateMachineController::StateMachineController(std::map<State, std::vector<ShipControl>> stateToShipControlInputsMap,
	std::map<State, std::map<Input, State>> stateWithInputToStateMap, int totalInputs, int deltaBeforeStateChange) :
	m_currentState(State0),
	m_previousState(InvalidState),
	m_timeOfLastStateChange(0),

	m_stateToShipControlInputsMap(stateToShipControlInputsMap),
	m_stateWithInputToStateMap(stateWithInputToStateMap),
	m_totalInputs(totalInputs),
	m_deltaBeforeStateChange(deltaBeforeStateChange)
{
}

bool StateMachineController::isItTimeToUpdateState(const sf::Int32& currentTime) const
{
	if (currentTime - m_timeOfLastStateChange < m_deltaBeforeStateChange)
		return false;
	return true;
}

void StateMachineController::updateControllerStateAndShipState(Ship& ship)
{
	const sf::Int32 currentTime = GameState::clock.getElapsedTime().asMilliseconds();
	bool isNewState = false;
	if (isItTimeToUpdateState(currentTime))
	{
		isNewState = true;
		m_timeOfLastStateChange = currentTime;
		const auto newInput = static_cast<Input>(rand() % m_totalInputs);

		//update state
		const auto inputToStateEntry = m_stateWithInputToStateMap.at(m_currentState).find(newInput);
		if (inputToStateEntry != m_stateWithInputToStateMap.at(m_currentState).end())
			m_currentState = inputToStateEntry->second;//if current state is new state, mov not reassigned
	}
	const std::vector<ShipControl>& currentShipControlVec = m_stateToShipControlInputsMap.at(m_currentState);

	if (!isNewState)
	{
		for (auto& shipControl : currentShipControlVec)
		{
			if(shipControl == FireWeapon1 || shipControl == FireWeapon2)
				ship.m_shipControlsStateMappings[shipControl] = true;
		}
		return;
	}
	

	//clear previous states commands
	for (auto& m_shipControlsStateMapping : ship.m_shipControlsStateMappings)
	{
		m_shipControlsStateMapping.second = false;
	}
	//update ship inputs based on new state
	
	for (auto& shipControl : currentShipControlVec)
	{
		ship.m_shipControlsStateMappings[shipControl] = true;
	}
}
