#include "LevelOrchestrator.h"

#include <conio.h>
#include <iostream>

#include "BackgroundManager.h"
#include "DebugMacros.h"
#include "GameState.h"

extern PlayerShip playerShip;
extern ProjectileManager enemyProjectileManager;
extern ProjectileManager playerProjectileManager;
extern ShipManager enemyShipsManager;
extern UIManager uiManager;
extern BackgroundManager backgroundManager;

LevelOrchestrator& LevelOrchestrator::addDrawableLayer(Drawable* drawable)
{
	m_drawables.push_back(drawable);
	return *this;
}

void LevelOrchestrator::initDefaultManager()
{

	addDrawableLayer(&backgroundManager)
		.addDrawableLayer(&playerProjectileManager)
		.addDrawableLayer(&enemyProjectileManager)
		.addDrawableLayer(&enemyShipsManager)
		.addDrawableLayer(&playerShip)
		.addDrawableLayer(&uiManager);
}

void LevelOrchestrator::initDefaultDrawableLayersAndOrder()
{
	addManager(&backgroundManager)
		.addManager(&playerProjectileManager)
		.addManager(&enemyProjectileManager)
		.addManager(&enemyShipsManager)
		.addManager(&playerShip)
		.addManager(&uiManager);
}


bool LevelOrchestrator::checkForGameEvent(KeyboardController& playerController) const
{
	if (GameState::requiresLevelRestart)
	{
		GameState::init();
		reset(playerController);
		return true;
	}
	if (GameState::isGameOver || GameState::isPaused)
		return true;
	return false;
}

LevelOrchestrator::LevelOrchestrator()
= default;

LevelOrchestrator& LevelOrchestrator::addManager(IManager* manager)
{
	m_managers.push_back(manager);
	return *this;
}

void LevelOrchestrator::reset(KeyboardController& playerController) const
{
	playerController.initKeyMappings();
	for(const auto& manager : m_managers)
	{
		manager->resetManager();
	}
}

bool LevelOrchestrator::shouldRunLoop()
{
	//Run game loop every X milliseconds
	if (GameState::clock.getElapsedTime().asMilliseconds() - GameState::timeOfLastGameLoop <= GAME_SPEED)
		return false;
	GameState::timeOfLastGameLoop = GameState::clock.getElapsedTime().asMilliseconds();
	return  true;
}

void LevelOrchestrator::updateWindow(sf::RenderWindow& window) const
{
	// Update the window, 
	window.clear();
	window.draw(*this);
	window.display();
}

void LevelOrchestrator::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& drawable : m_drawables)
	{
		target.draw(*drawable);
	}
}

void LevelOrchestrator::pollForMovementSetting(sf::RenderWindow& window)
{
	if (GameState::isMovementSet)
		return;

	using namespace std;
	ShowConsole();
	while (true) {
		posePlayerQuestion();
		auto playerInput = _getch();
		const int intValue = playerInput - '1';

		switch (intValue)
		{
		case GameState::movement_control::full_ship_orientation:
		case GameState::movement_control::window_and_ship_orientation:
		case GameState::movement_control::full_window_orientation:
			GameState::movementControlSetting = static_cast<GameState::movement_control>(intValue);
			GameState::isMovementSet = true;
#ifdef NDEBUG
			HideConsole();
#endif
#ifdef _DEBUG
			ShowConsole();
#endif
			window.requestFocus();
			return;
		default:
			cout << "Not a valid option." << endl;
		}
	}
}


int LevelOrchestrator::loadLevel(std::shared_ptr<ILevel> level)
{
	if (level->initializeLevel() == EXIT_FAILURE)
		return EXIT_FAILURE;

	m_level = level;
	return 0;
}

void LevelOrchestrator::initializeLevelIntroText(UIManager& uiManager) const
{
	uiManager.initializeLevelIntroText(m_level->level_into_text_primary, m_level->level_into_text_secondary);
}

void LevelOrchestrator::initializeLevelOutroText(UIManager& uiManager) const
{
	uiManager.initializeLevelOutroText(m_level->level_outro_text_primary, m_level->level_outro_text_secondary);
}

void LevelOrchestrator::enemyShipCreation() const
{
	m_level->enemyShipCreation();
}


void LevelOrchestrator::posePlayerQuestion()
{
	using namespace std;
	std::cout << "Please Select a Movement Control Option\n(enter associated number)" << endl;
	std::cout << "[1] Full Window Orientation" << endl;
	std::cout << "[2] Ship Vertical Orientation with Window Horizontal Orientation(recommended)" << endl;
	std::cout << "[3] Full Ship Orientation" << endl;
}