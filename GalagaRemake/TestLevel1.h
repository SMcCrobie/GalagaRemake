#pragma once
#include "ILevel.h"

class TestLevel1
	: public ILevel
{
public:
	void initializeLevel() override;

	void updateLevel() override;

	void killLevel() override;

	void resetLevel() override;
};

