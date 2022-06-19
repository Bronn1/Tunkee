#pragma once

#include "game_tile.h"
#include "unit.h"
#include "game_command.h"

#include <unordered_map>
#include <vector>
#include <memory>

using ActionPtr = std::shared_ptr<GameAction>;

namespace core
{
namespace details
{
constexpr int getStandardSetupAreaHeight(const int height);
} //namespace details
class GameBoard
{
public:
    using SetupAreaVec = std::vector<GameTile>;
    GameBoard() = default;
    GameBoard(std::vector<GameTileType> tiles, int height, int width);

    std::vector<GameTile> getStraightLine(const GameTile& from, const GameTile& to) const;
    std::vector<GameTile> getLineOfSightWithoutObstacles(const Unit* unit, const GameTile& dest) const;

    inline GameTile getTile(const  GameTile& tile) const
    {
        if (m_tiles.contains(tile)) return m_tiles.at(tile);
        else return NullTile();
    }

    std::vector<GameTile> getSetupArea(const PlayerIdentifier playerId) const;
    const std::vector<GameTile>& getTileCoordinates() { return m_orderedBoard; }

    inline void setTileType(const GameTile& tile, const GameTileType type)
    {
        if (m_tiles.contains(tile))
            m_tiles.at(tile).m_type = type;
    }

    inline void setTileAccessible(const GameTile& tile, bool isAccessible)
    {
        if (m_tiles.contains(tile))
            m_tiles.at(tile).isAccessible = isAccessible;
    }

    bool isAccessible(const GameTile& tile) const;
    inline bool contains(const GameTile& tile) const { return  m_tiles.contains(tile); }


    void initSetupArea(const PlayerIdentifier playerId, const  SetupAreaVec& setupArea);
    bool isInsideSetupArea(const PlayerIdentifier playerId, const  GameTile& tile) const;

    void printBoardTest();

    ~GameBoard() = default;

    std::vector<core::GameTile> getUpperLines(int rowAmount);
    std::vector<core::GameTile> getBottomLines(int rowAmount);
private:
    int m_height{ 0 };
    int m_width{ 0 };
    /*! mostly for pathfinding purposes */
    std::unordered_map<GameTile, GameTile> m_tiles{};
    std::unordered_map<PlayerIdentifier, SetupAreaVec> m_setupAreas{};
    /*! for visual board representaion*/
    std::vector<GameTile> m_orderedBoard{};
};
}// namespace core 
