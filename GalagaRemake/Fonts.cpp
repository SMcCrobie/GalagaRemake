#include "Fonts.h"
#include "Loader.h"

namespace Fonts
{
	sf::Font galaxus;
	sf::Font play_fair;
	sf::Font roboto;

	void load()
	{
		Loader::LOAD_SAFELY(galaxus, R"(Assets\Fonts\ClimateCrisis-Regular.ttf)");
		Loader::LOAD_SAFELY(play_fair, R"(Assets\Fonts\PlayfairDisplay.ttf)");
		Loader::LOAD_SAFELY(roboto, R"(Assets\Fonts\RobotoMedium.ttf)");
	}

	

}
