#pragma once

namespace GameState
{
	inline bool isGameOver, isPaused, requiresLevelRestart, isBossCreated, isBossDestroyed, isPlayerDead, isKeyTrapActivated, isIntroDone;
	inline int killCounter, gameCycleCounter, timeOfLastGameLoop, timeOfLastEnemyShip, deltaTillNextEnemyShip, levelOutroDelay;
	inline sf::Clock clock;


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

		killCounter = 0;
		gameCycleCounter = 0;
		timeOfLastGameLoop = 0;
		timeOfLastEnemyShip = 0;
		deltaTillNextEnemyShip = 300;
		levelOutroDelay = -1; 

		clock.restart();
	}
}
