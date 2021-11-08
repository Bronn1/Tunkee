#include "game_board.h"
#include "pathfinding.h"

#include <iostream>
#include <algorithm>
#include <optional>

core::GameBoard::GameBoard(std::vector<GameTileType> tiles, int width, int height) : m_width(width-1), m_height(height-1)
{
	//m_tiles.insert({ GameTile(1, 0), GameTile(1, 0) });
	//m_tiles.insert({ GameTile(2, 0), GameTile(2, 0) });
	//std::cout << m_tiles[GameTile(1, -1)] << "tyt\n";
	//std::cout << (GameTile(2, -1) == GameTile(2, -1)) << "ty1t\n";
	auto begin = core::GameTile(0, 2);
	auto end = core::GameTile(0, -1);
	//std::cout << pathfinding::getDistance(begin, end) << "\n";
	constexpr int top = 0;
	constexpr int left = 0;

	for (int r = top; r <= m_width; r++) { // pointy top
		int r_offset = floor(r / 2.0); // or r>>1
		for (int q = left - r_offset; q <= m_height - r_offset; q++) {
			if (m_tiles.contains(core::GameTile(q, r))) {
				std::cout << "fFFFF\n";
			}
			//std::cout << core::GameTile(q, r) << "\n";
			m_tiles.insert({ GameTile(q, r), GameTile(q, r) });
			m_orderedBoard.push_back(GameTile(q, r));
		}
	}
	auto result = pathfinding::getNeighbors(*this, begin);
	for (auto& p : result)
	{
		//std::cout << p << "\n";
	}

	m_unitManager = std::make_shared_for_overwrite<UnitManager>();
	//m_playerOne =  std::make_shared<Player>(Player());
	//std::cout << result << "\n";
	//printBoardTest();

}


void core::GameBoard::getMoveAreaForUnit(const GetMoveAreaQuery* getAreaCmd)
{
	auto unit = (*m_unitManager).getUnitIfExist(getAreaCmd->m_unitID);

	if (unit)
	{
		// full distance and half distance should be printed with different colors in gui
		TileDistance halfMovement = unit.value()->getHalfMovePoints();
		TileDistance remainingMovement = unit.value()->getRemainingMovement();
		std::vector<GameTile> moveArea = {};
		int halfAreaSize;
		if (remainingMovement > halfMovement)
		{
			moveArea = pathfinding::getAvailableArea(*this, unit.value()->getPosition(), halfMovement);
			halfAreaSize = std::size(moveArea);
			auto  fullMoveArea = pathfinding::getAvailableArea(*this, unit.value()->getPosition(), remainingMovement);
			for (auto& tile : fullMoveArea)
			{
				//v2.insert(v2.end(), std::make_move_iterator(v1.begin() + 7), std::make_move_iterator(v1.end()));
				if (std::ranges::find(moveArea, tile) == std::end(moveArea))
					moveArea.push_back(tile);
			}
		}
		else {
			moveArea = pathfinding::getAvailableArea(*this, unit.value()->getPosition(), remainingMovement);
			halfAreaSize = std::size(moveArea);
		}
		std::cout << std::size(moveArea) << "SIZE\n"; //notify observers
		notifyTileEventObservers(unit.value()->getID(), getAreaCmd->m_name, moveArea, halfAreaSize);
	}
}

bool core::GameBoard::moveTo(const MoveToAction* moveToCmd)
{
	auto currentPlayer = getCurrentPlayerId();
	auto unit = (*m_unitManager).getUnitIfExist(moveToCmd->m_unitID);

	if (unit && unit.value()->getOwnerID() == currentPlayer)
	{
		GameTile currentPos = unit.value()->getPosition();
		auto pathVec = pathfinding::getShortestPath(*this, currentPos, getTile(moveToCmd->m_destination));
		auto adjustedPathVec = unit.value()->moveTo(pathVec);
		if (std::size(adjustedPathVec) > 0)
		{
			setTileAccessible(currentPos, true);
			setTileAccessible(adjustedPathVec.back(), false); //notify
			notifyTileEventObservers(unit.value()->getID(), moveToCmd->m_name, adjustedPathVec);
			return true;
		}
	}
	return false;
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
