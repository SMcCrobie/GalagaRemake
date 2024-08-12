#include "MainMenuLevel.h"

#include "GameState.h"
#include "UIManager.h"


//extern PlayerShip playerShip;
extern UIManager uiManager;

void MainMenuLevel::initializeLevel()
{
	uiManager.hideUI();
	uiManager.initMainMenuText();
	GameState::isMainMenu = true;
}

void MainMenuLevel::updateLevel()
{
	
}

void MainMenuLevel::killLevel()
{
}

void MainMenuLevel::resetLevel()
{
}
