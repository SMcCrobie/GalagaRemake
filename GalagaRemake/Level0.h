#pragma once
#include "ILevel.h"
class Level0 :
    public ILevel
{
public:
	void initializeLevel() override;

	void enemyShipCreation() override;

	void killLevel() override;

	void resetLevel() override;
};

