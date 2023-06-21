#pragma once
#include "ILevel.h"
class Level0 :
    public ILevel
{
public:
	int initializeLevel() override;

	void enemyShipCreation(std::shared_ptr<ShipManager>& enemyShipsManager) override;

	void killLevel() override;
private:
	sf::Texture bossAnimations;
	int timeOfLastEnemyShip = 0;
	int deltaTillNextEnemyShip = 0;
	bool isBossCreated = false;

	Ship bossShip;
	CircleProjectile bossProjectile = CircleProjectile();
	std::shared_ptr<sf::Texture> bossProjectileTexture;
	sf::Color shieldColor;

	std::map<State, std::vector<ShipControl>> bossStateToShipControlInputsMap;
	std::map<State, std::map<Input, State>> bossStateWithInputToStateMap;

};

