#pragma once
#include "ShipManager.h"
#include "TempText.h"

class ILevel
{
public:
	virtual ~ILevel() = default;
	virtual void initializeLevel() = 0;

	virtual void updateLevel() = 0;

	virtual void killLevel() = 0 ;

	virtual void resetLevel() = 0;

	TempText level_into_text_primary;
	TempText level_into_text_secondary;

	TempText level_outro_text_primary;
	TempText level_outro_text_secondary;
};
