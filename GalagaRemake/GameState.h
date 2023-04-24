#pragma once

namespace GameState
{
	enum movement_control { full_window_orientation, window_and_ship_orientation, full_ship_orientation };

	inline bool isGameOver, isPaused, requiresLevelRestart, isBossCreated, isBossDestroyed, isPlayerDead, isKeyTrapActivated, isIntroDone;
	inline int killCounter, gameCycleCounter, timeOfLastGameLoop, timeOfLastEnemyShip, deltaTillNextEnemyShip, levelOutroDelay;
	inline sf::Clock clock;
	inline movement_control movementControlSetting = window_and_ship_orientation;


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

		killCounter = 40;
		gameCycleCounter = 0;
		timeOfLastGameLoop = 0;
		timeOfLastEnemyShip = 0;
		deltaTillNextEnemyShip = 300;
		levelOutroDelay = -1; 

		clock.restart();
	}
}
