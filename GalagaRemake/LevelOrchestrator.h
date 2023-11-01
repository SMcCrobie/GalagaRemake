#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "ILevel.h"
#include "IManager.h"
#include "UIManager.h"

#define GAME_SPEED 20

class LevelOrchestrator
	: public sf::Drawable
{
public:
	LevelOrchestrator();

	LevelOrchestrator& addManager(IManager* manager);
	LevelOrchestrator& addDrawableLayer(Drawable* drawable);
	void initDefaultManager();
	void initDefaultDrawableLayersAndOrder();
	bool checkForGameEvent(KeyboardController& playerController) const;
	void reset(KeyboardController& playerController) const;
	static bool shouldRunLoop();
	void updateWindow(sf::RenderWindow& window) const;
	static void pollForMovementSetting(sf::RenderWindow& window);

	int loadLevel(std::shared_ptr<ILevel> level);
	void initializeLevelIntroText(UIManager& uiManager) const;
	void initializeLevelOutroText(UIManager& uiManager) const;
	void enemyShipCreation() const;


private:
	std::vector<Drawable*> m_drawables;
	std::vector<IManager*> m_managers;
	std::shared_ptr<ILevel> m_level;

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	static void posePlayerQuestion();
};

