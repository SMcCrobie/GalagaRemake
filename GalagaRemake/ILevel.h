#pragma once
#include "ShipManager.h"
#include "TempText.h"

class ILevel
{
public:
	virtual int initializeLevel() = 0;

	virtual void enemyShipCreation() = 0;

	virtual void killLevel() = 0 ;

	TempText level_into_text_primary;
	TempText level_into_text_secondary;

	TempText level_outro_text_primary;
	TempText level_outro_text_secondary;
};


