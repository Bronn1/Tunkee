#include "game_board.h"
#include "pathfinding.h"

#include <iostream>
#include <algorithm>
#include <optional>

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

MoveAreaInfo core::GameBoard::getMoveAreaForUnit(const GetMoveAreaQuery* getAreaCmd, Unit* unit)
{
	// full distance and half distance should be printed with different colors in game
	TileDistance halfMovement = unit->getHalfMovePoints();
	TileDistance remainingMovement = unit->getRemainingMovement();
	TileDistance remainingMovementInFirstHalf = unit->getRemainingMovementInFirstHalf();
	std::vector<GameTile> moveArea = {};
	int firstLayerSize = 0;

	if (remainingMovement >= halfMovement) 
	{
		moveArea = pathfinding::getAvailableArea(*this, unit->getPosition(), remainingMovementInFirstHalf);
		firstLayerSize = std::size(moveArea);
		auto  fullMoveArea = pathfinding::getAvailableArea(*this, unit->getPosition(), remainingMovement);
		for (auto& tile : fullMoveArea)
		{
			//v2.insert(v2.end(), std::make_move_iterator(v1.begin() + 7), std::make_move_iterator(v1.end()));
			// add only new(unique) tiles
			if (std::ranges::find(moveArea, tile) == std::end(moveArea))
				moveArea.push_back(tile);
		}
	}
	else {
		moveArea = pathfinding::getAvailableArea(*this, unit->getPosition(), remainingMovement);
		firstLayerSize = 0;
	}
	return MoveAreaInfo{ std::move(moveArea), firstLayerSize };//moveArea;
}

std::vector<core::GameTile> core::GameBoard::moveTo(const MoveToAction* moveToCmd, Unit* unit)
{
	auto currentPlayer = getCurrentPlayerId();

	if (unit->getOwnerID() == currentPlayer)
	{
		GameTile currentPos = unit->getPosition();
		auto pathVec = pathfinding::getShortestPath(*this, currentPos, getTile(moveToCmd->m_destination));
		auto adjustedPathVec = unit->moveTo(pathVec);
		if (std::size(adjustedPathVec) > 0)
		{
			setTileAccessible(currentPos, true);
			setTileAccessible(adjustedPathVec.back(), false);
			return adjustedPathVec;
		}
	}
	return std::vector<core::GameTile>();
}

void core::GameBoard::printBoardTest()
{
	int i = 0, j = 0;
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
