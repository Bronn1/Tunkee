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
    QuickGame
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
struct LaunchSettings
{
    PlayerIdentifier m_host{ {0} };
    PlayerIdentifier m_guest{ {0} };;
    setup::Money amount;
    GameType type;
    MapSize mapSize;
   // SetupAreaType setupAreaType;
    core::GameBoardType boardType;
    std::vector<core::GameTile> setupArea{};
};

} // namespace settings