#pragma once

class IManager
{
public:
	virtual ~IManager() = default;
	virtual void resetManager() = 0;
	
};
