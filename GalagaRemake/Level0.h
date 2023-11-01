#pragma once
#include "ILevel.h"
class Level0 :
    public ILevel
{
public:
	int initializeLevel() override;

	void enemyShipCreation() override;

	void killLevel() override;

};

