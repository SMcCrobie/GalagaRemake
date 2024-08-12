#include "Fonts.h"
#include "Loader.h"

namespace Fonts
{
	sf::Font galaxus;
	sf::Font play_fair;
	sf::Font roboto;

	void load()
	{
		Loader::LOAD_SAFELY(galaxus, "ClimateCrisis-Regular.ttf");
		Loader::LOAD_SAFELY(play_fair, "PlayfairDisplay.ttf");
		Loader::LOAD_SAFELY(roboto, "RobotoMedium.ttf");
	}

	

}
