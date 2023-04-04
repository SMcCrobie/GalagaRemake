#pragma once
#include <stdexcept>
#include <string>
#include "DebugMacros.h"

namespace Loader
{
	template<typename LoaderClass>
	inline void LOAD_SAFELY(LoaderClass& loader, const std::string& filePath)
	{
		
		if (!loader.loadFromFile(filePath))
		{
			ShowConsole();
			throw std::invalid_argument("Failed to load " + filePath);
		}
	}

}
