#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "ILevel.h"
#include "Manager.h"
#include "UIManager.h"

#define GAME_SPEED 20

class LevelManager
	: public sf::Drawable
{
public:
	LevelManager();

	LevelManager& addManager(const std::shared_ptr<Manager>& manager);
	LevelManager& addDrawableLayer(const std::shared_ptr<sf::Drawable>& drawable);
	bool checkForGameEvent(KeyboardController& playerController) const;
	void reset(KeyboardController& playerController) const;
	static bool shouldRunLoop();
	void updateWindow(sf::RenderWindow& window) const;
	void pollForMovementSetting(sf::RenderWindow& window);

	int loadLevel(std::shared_ptr<ILevel> level);
	void initializeLevelIntroText(UIManager& uiManager) const;
	void initializeLevelOutroText(UIManager& uiManager) const;
	void enemyShipCreation(std::shared_ptr<ShipManager>& enemyShipsManager) const;


private:
	std::vector<std::shared_ptr<sf::Drawable>> m_drawables;
	std::vector<std::shared_ptr<Manager>> m_managers;
	std::shared_ptr<ILevel> m_level;

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	static void posePlayerQuestion();
};

