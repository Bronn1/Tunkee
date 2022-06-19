#include "game_board.h"
#include "pathfinding.h"

#include <iostream>
#include <algorithm>
#include <ranges>

namespace views = std::views;
namespace ranges = std::ranges;

namespace core {
constexpr int details::getStandardSetupAreaHeight(const int height)
{
    constexpr int kMediumMapHeight = 30;
    if (height < kMediumMapHeight) {
        return 1;
    }

    return 2;
}

GameBoard::GameBoard(std::vector<GameTileType> tiles, int height, int width) : m_width(width), m_height(height)
{
    constexpr int top = 0;
    constexpr int left = 0;

    for (int r = top; r < m_height; r++) { // pointy top
        int r_offset = floor(r / 2.0); // or r>>1
        for (int q = left - r_offset; q < m_width - r_offset; q++) {
            if (m_tiles.contains(core::GameTile(q, r))) {
                std::cout << "fFFFF\n"; // throw custom exception 
            }
            m_tiles.insert({ GameTile(q, r), GameTile(q, r) });
            m_orderedBoard.push_back(GameTile(q, r));
        }
    }
}

std::vector<GameTile> GameBoard::getStraightLine(const GameTile& from, const GameTile& to) const
{
    return pathfinding::drawLine(from, to);
}

std::vector<GameTile> GameBoard::getLineOfSightWithoutObstacles(const Unit* unit, const GameTile& dest) const
{
    int distance = pathfinding::getDistance(unit->getPosition(), dest);
    // so after one turret rotation in unit we should disallow rotation to more than 120
    // if turret cant rotate then check if destination inside line of sight(120degree), make angleToclosestVertex;
    return pathfinding::getLineOfSightWithoutObstacles(*this, unit->getPosition(), TileDistance{ (unsigned)distance + 5 }, AngleToClosestVertex(unit->getGunRotation()));
}

std::vector<GameTile> GameBoard::getSetupArea(const PlayerIdentifier playerId) const
{
    if (!m_setupAreas.contains(playerId)) {
        return std::vector<GameTile>();
    }
    
    return m_setupAreas.at(playerId);
}

bool GameBoard::isAccessible(const GameTile& tile) const
{
    if (!m_tiles.contains(tile)) {
        return false;
    }

    return  m_tiles.at(tile).isAccessible;
}

void GameBoard::initSetupArea(const PlayerIdentifier playerId, const SetupAreaVec& setupArea)
{
    auto setupAreaVec = setupArea;
    if (std::size(setupAreaVec) == 0) {
        int setupLinesCount = details::getStandardSetupAreaHeight(m_height);
        setupAreaVec = std::size(m_setupAreas) == 0 ? getUpperLines(setupLinesCount) : getBottomLines(setupLinesCount);
    }

    m_setupAreas.insert({ playerId, std::move(setupAreaVec) });
}

bool GameBoard::isInsideSetupArea(const PlayerIdentifier playerId, const GameTile& tile) const
{
    if (!m_setupAreas.contains(playerId)) {
        return false;
    }
    //std::copy(begin(m_setupAreas.at(playerId)), end(m_setupAreas.at(playerId)), std::ostream_iterator<GameTile>(std::cout, ""));

    return ranges::count(m_setupAreas.at(playerId), tile);
}

void GameBoard::printBoardTest()
{
    int i = 0;
    std::cout << std::size(m_orderedBoard) << "\n";
    for (auto& p : m_orderedBoard) {
        std::cout << p << ", ";
        i++;
        if (i > m_height) {
            std::cout << "\n";
            i = 0;
        }
    }
}

std::vector<core::GameTile> GameBoard::getUpperLines(int rowAmount)
{
    std::vector<GameTile> rows{};
    for (int i = 0; i < m_width * rowAmount && i < size(m_orderedBoard); ++i) {
        rows.push_back(m_orderedBoard[i]);
    }

    return rows;
}

std::vector<core::GameTile> GameBoard::getBottomLines(int rowAmount)
{
    std::vector<GameTile> rows{};
    for (int i = size(m_orderedBoard) - m_width * rowAmount; i >= 0 && i < size(m_orderedBoard); ++i) {
        //int index = 
        rows.push_back(m_orderedBoard[i]);
    }

    return rows;
}


} // namespace core 

