#include "Fonts.h"
#include "Loader.h"

namespace Fonts
{
	sf::Font galaxus;
	sf::Font playFair;


	void load()
	{
		Loader::LOAD_SAFELY(galaxus, "ClimateCrisis-Regular.ttf");
		Loader::LOAD_SAFELY(playFair, "PlayfairDisplay.ttf");
	}

	

}
