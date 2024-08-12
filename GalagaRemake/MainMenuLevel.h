#pragma once
#include "ILevel.h"
class MainMenuLevel :
    public ILevel
{
public:
	void initializeLevel() override;

	void updateLevel() override;

	void killLevel() override;

	void resetLevel() override;
};

