#pragma once

namespace GameState
{
	inline bool isGameOver = false;
	inline bool isPaused = false;
	inline bool requiresLevelRestart = false;
	inline bool isBossCreated = false;
	inline bool isBossDestroyed = false;
	inline bool isPlayerDead = false;

	inline int killCounter = 0;
	inline int gameCycleCounter = 0;
	inline int timeOfLastGameLoop = 0;
	inline int timeOfLastEnemyShip = -1000;
	inline int deltaTillNextEnemyShip = 6000;
	inline int levelOutroDelay = -1;

	//Clock
	inline sf::Clock clock;


	inline void init()
	{
		isGameOver = false;
		isPaused = false;
		requiresLevelRestart = false;
		isBossCreated = false;
		isBossDestroyed = false;
		isPlayerDead = false;

		killCounter = 0;
		gameCycleCounter = 0;
		timeOfLastGameLoop = 0;
		timeOfLastEnemyShip = -1000;
		deltaTillNextEnemyShip = 6000;
		levelOutroDelay = -1; 

		clock.restart();
	}
}
