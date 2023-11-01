#pragma once
#include "ILevel.h"
#include "TempText.h"

class Level1 :
	public ILevel
{
public:
	int initializeLevel() override;

	void enemyShipCreation() override;

	void killLevel() override;

};

