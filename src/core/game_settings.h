#pragma once

#include "data_types.h"

namespace core // TODO  Board(map) generator isn't ready yet, so i will leave map type here for now 
{
enum class GameBoardType
{
    Plain
};
} //namespace core

namespace settings
{
enum class GameType
{
    QuickGame,
    Multiplayer
};

enum class SetupAreaType
{
    Standard,
    Custom
};

struct MapSize
{
    int width{ 40 };
    int height{ 40 };
};

/**
 * @brief Settings that are necessary for the correct launch of the game
*/
struct GameSettings
{
    std::vector<core::GameTile> setupArea{};
    std::string hostName{""};
    std::string guestName{ "" };
    MapSize mapSize;
    PlayerIdentifier hostId{ {0} };
    PlayerIdentifier guestId{ {0} };
    setup::Money amount;
    GameType type{ GameType::QuickGame };
   // SetupAreaType setupAreaType;
    core::GameBoardType boardType{ core::GameBoardType::Plain };
};

} // namespace settings
