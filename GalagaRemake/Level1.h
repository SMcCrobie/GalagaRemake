#pragma once
#include "ILevel.h"
#include "TempText.h"

class Level1 :
	public ILevel
{
public:
	int initializeLevel() override;

	void enemyShipCreation(std::shared_ptr<ShipManager>& enemyShipsManager) override;

	void killLevel() override;

private:

	//textures
	sf::Texture shipAnimations;
	sf::Texture bossAnimations;
	sf::Texture bossSideKicksAnimations;
	std::shared_ptr<sf::Texture> bossProjectileTexture;

	//objects
	Ship enemyShip;
	Ship bossShip;
	Ship bossSideKicks;
	CircleProjectile bossProjectile = CircleProjectile();
	RectangleProjectile enemyProjectile = RectangleProjectile(sf::Vector2f(3.f, 12.f));
	sf::Color shieldColor;

	//controllers
	std::map<State, std::vector<ShipControl>> bossSideKicksStateToShipControlInputsMap;
	std::map<State, std::vector<ShipControl>> bossSideKicksStateToShipControlInputsMapB;
	std::map<State, std::map<Input, State>> bossSideKicksStateWithInputToStateMap;
	StateMachineController bossSideKicksController;
	StateMachineController bossSideKicksControllerB;


	std::map<State, std::vector<ShipControl>> bossStateToShipControlInputsMap;
	std::map<State, std::map<Input, State>> bossStateWithInputToStateMap;

	StateMachineController bossController;

};

