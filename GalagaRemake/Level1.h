#pragma once
#include "ILevel.h"
#include "TempText.h"

class Level1 :
	public ILevel
{
public:
	void initializeLevel() override;

	void updateLevel() override;

	void killLevel() override;

	void resetLevel() override;

};

