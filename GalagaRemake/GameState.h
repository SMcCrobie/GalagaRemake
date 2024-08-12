#pragma once
#include "BoundedFloatRect.h"

namespace GameState
{
	enum movement_control { full_window_orientation, window_and_ship_orientation, full_ship_orientation };

	inline bool isGameOver, isPaused, requiresLevelRestart, isBossCreated, isBossDestroyed, isPlayerDead, isKeyTrapActivated, isIntroDone, resetWindow, ignoreNextResizeEvent, isMainMenu;
	inline int killCounter, gameCycleCounter, timeOfLastGameLoop, timeOfLastEnemyShip, deltaTillNextEnemyShip, levelOutroDelay, score;
	inline sf::Clock clock;
	inline movement_control movementControlSetting = window_and_ship_orientation;
	inline bool isMovementSet = false;

	inline BoundedFloatRect world_bounds(0.0f, 0.0f, 600.0f, 1000.0f);
	inline sf::View world_view(world_bounds);

	inline void init()
	{
		isGameOver = false;
		isPaused = false;
		requiresLevelRestart = false;
		isBossCreated = false;
		isBossDestroyed = false;
		isPlayerDead = false;
		isKeyTrapActivated = false;
		isIntroDone = false;
		resetWindow = false;
		ignoreNextResizeEvent = false;
		isMainMenu = false;//finale straw, this is going to get bad

		score = 0;
		killCounter = 0;
		gameCycleCounter = 0;
		timeOfLastGameLoop = 0;
		timeOfLastEnemyShip = 0;
		deltaTillNextEnemyShip = 300;//100, need to make this level specific
		levelOutroDelay = -1;

		world_bounds = BoundedFloatRect (0.0f, 0.0f, 600.0f, 1000.0f);
		world_view.setViewport(world_bounds);

		clock.restart();
	}
}
