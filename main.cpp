#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include "src/tunkee_game.h"

#include <stdexcept>
#include <iostream>


int main()
{
	try
	{
		TunkeGame tunkee;
		tunkee.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
}