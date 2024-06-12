#pragma once
#include <stdexcept>
#include <iostream>
#include <string>
#include <cassert>
#include "DebugMacros.h"

namespace Loader
{
    template<typename LoaderClass>
    typename std::enable_if<std::is_same<decltype(std::declval<LoaderClass>().loadFromFile(std::declval<const std::string&>())), bool>::value>::type
	inline LOAD_SAFELY(LoaderClass& loader, const std::string& filePath)
    {
       
        if (!loader.loadFromFile(filePath))//prints its own log
        {
            //better then what I had, but maybe add a validator, because the assests will only attempt load when the specifc level loads
        	ShowConsole();
            std::cin.get();
            abort();
        }
    }
}
