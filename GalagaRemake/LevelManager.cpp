#include "LevelManager.h"

#include <conio.h>
#include <iostream>

#include "DebugMacros.h"
#include "GameState.h"

LevelManager& LevelManager::addDrawableLayer(const std::shared_ptr<sf::Drawable>& drawable)
{
	m_drawables.push_back(drawable);
	return *this;
}



bool LevelManager::checkForGameEvent(KeyboardController& playerController) const
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

LevelManager::LevelManager()
{


}

LevelManager& LevelManager::addManager(const std::shared_ptr<Manager>& manager)
{
	m_managers.push_back(manager);
	return *this;
}

void LevelManager::reset(KeyboardController& playerController) const
{
	playerController.initKeyMappings();
	for(const auto& manager : m_managers)
	{
		manager->resetManager();
	}
}

bool LevelManager::shouldRunLoop()
{
	//Run game loop every X milliseconds
	if (GameState::clock.getElapsedTime().asMilliseconds() - GameState::timeOfLastGameLoop <= GAME_SPEED)
		return false;
	GameState::timeOfLastGameLoop = GameState::clock.getElapsedTime().asMilliseconds();
	return  true;
}

void LevelManager::updateWindow(sf::RenderWindow& window) const
{
	// Update the window, 
	window.clear();
	window.draw(*this);
	window.display();
}

void LevelManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& drawable : m_drawables)
	{
		target.draw(*drawable);
	}
}

void LevelManager::pollForMovementSetting(sf::RenderWindow& window)
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
			//GameState::init();
			return;
		default:
			cout << "Not a valid option." << endl;
		}
	}
}


int LevelManager::loadLevel(std::shared_ptr<ILevel> level)
{
	if (level->initializeLevel() == EXIT_FAILURE)
		return EXIT_FAILURE;

	m_level = level;
	return 0;
}

void LevelManager::initializeLevelIntroText(UIManager& uiManager) const
{
	uiManager.initializeLevelIntroText(m_level->level_into_text_primary, m_level->level_into_text_secondary);
}

void LevelManager::initializeLevelOutroText(UIManager& uiManager) const
{
	uiManager.initializeLevelOutroText(m_level->level_outro_text_primary, m_level->level_outro_text_secondary);
}

void LevelManager::enemyShipCreation(std::shared_ptr<ShipManager>& enemyShipsManager) const
{
	m_level->enemyShipCreation(enemyShipsManager);
};


void LevelManager::posePlayerQuestion()
{
	using namespace std;
	std::cout << "Please Select a Movement Control Option\n(enter associated number)" << endl;
	std::cout << "[1] Full Window Orientation" << endl;
	std::cout << "[2] Ship Vertical Orientation with Window Horizontal Orientation(recommended)" << endl;
	std::cout << "[3] Full Ship Orientation" << endl;
}