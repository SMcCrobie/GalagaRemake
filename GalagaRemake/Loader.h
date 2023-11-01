#pragma once
#include <stdexcept>
#include <string>
#include "DebugMacros.h"

namespace Loader
{
    template<typename LoaderClass>
    typename std::enable_if<std::is_same<decltype(std::declval<LoaderClass>().loadFromFile(std::declval<const std::string&>())), bool>::value>::type
	inline LOAD_SAFELY(LoaderClass& loader, const std::string& filePath)
    {
        if (!loader.loadFromFile(filePath))
        {
            ShowConsole();
            throw std::invalid_argument("Failed to load " + filePath);
        }
    }
}
