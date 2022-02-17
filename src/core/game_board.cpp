#include "game_board.h"
#include "pathfinding.h"

#include <iostream>
#include <algorithm>
#include <ranges>

namespace views = std::views;
namespace ranges = std::ranges;

core::GameBoard::GameBoard(std::vector<GameTileType> tiles, int width, int height) : m_width(width-1), m_height(height-1)
{
    constexpr int top = 0;
    constexpr int left = 0;

    for (int r = top; r <= m_width; r++) { // pointy top
        int r_offset = floor(r / 2.0); // or r>>1
        for (int q = left - r_offset; q <= m_height - r_offset; q++) {
            if (m_tiles.contains(core::GameTile(q, r))) {
                std::cout << "fFFFF\n"; // throw custom exception 
            }
            m_tiles.insert({ GameTile(q, r), GameTile(q, r) });
            m_orderedBoard.push_back(GameTile(q, r));
        }
    }
}

std::vector<GameTile> core::GameBoard::getStraightLine(const GameTile& from, const GameTile& to) const
{
    return pathfinding::drawLine(from, to);
}

std::vector<GameTile> core::GameBoard::getLineOfSightWithoutObstacles(const Unit* unit, const GameTile& dest) const
{
    int distance = pathfinding::getDistance(unit->getPosition(), dest);
    // so after one turret rotation in unit we should disallow rotation to more than 120
    // if turret cant rotate then check if destination inside line of sight(120degree), make angleToclosestVertex;
    return pathfinding::getLineOfSightWithoutObstacles(*this, unit->getPosition(), TileDistance{ (unsigned)distance + 5 }, AngleToClosestVertex( unit->getGunRotation()));
}

void core::GameBoard::printBoardTest()
{
    int i = 0;
    std::cout << std::size(m_orderedBoard) << "\n";
    for (auto& p : m_orderedBoard)
    {
        std::cout << p << ", ";
        i++;
        if (i > m_height) {
            std::cout << "\n";
            i = 0;
        }
    }
}
