#include "src/tunkee_game.h"

int main()
{
    try
    {
        TunkeGame tunkee;
        tunkee.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nEXCEPTION: " << e.what() << std::endl;
    }
}