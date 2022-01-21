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

MoveAreaInfo core::GameBoard::getMoveAreaForUnit(Unit *unit)
{
	TileDistance halfMovement = unit->getHalfMovePointsRoundedUp();
	TileDistance remainingMovement = unit->getRemainingMovement();
	std::vector<GameTile> moveArea = {};
	int firstLayerSize = 0;

	// full Movement and half Movement should be showed with different colors in game
	if (remainingMovement >= halfMovement ) {
		TileDistance remainingMovementInFirstHalf = unit->getRemainingMovementInFirstHalf();
		moveArea = pathfinding::getAvailableAreaWithRotation(*this, unit->getPosition(), remainingMovementInFirstHalf, unit->getUnitVertexRotation());
		firstLayerSize = std::size(moveArea);
		auto  fullMoveArea = pathfinding::getAvailableAreaWithRotation(*this, unit->getPosition(), remainingMovement, unit->getUnitVertexRotation());
		for (auto& tile : fullMoveArea)
			// add only new(unique) tiles
			if (std::ranges::find(moveArea, tile) == std::end(moveArea)) 
				moveArea.push_back(tile);
	}
	else {
		moveArea = pathfinding::getAvailableAreaWithRotation(*this, unit->getPosition(), remainingMovement, unit->getUnitVertexRotation());
		firstLayerSize = 0;
	}
	return MoveAreaInfo{ std::move(moveArea), firstLayerSize };//moveArea;
}

std::vector<core::GameTile> core::GameBoard::moveTo(const MoveToAction* moveToCmd, Unit* unit)
{
	rotateToDestination(moveToCmd, unit);

	GameTile currentPos = unit->getPosition();
	auto pathVec = pathfinding::getShortestPath(*this, currentPos, getTile(moveToCmd->m_destination));
	auto adjustedPathVec = unit->moveTo(pathVec);
	if (std::size(adjustedPathVec) > 0)
	{
		setTileAccessible(currentPos, true);
		setTileAccessible(adjustedPathVec.back(), false);
		return adjustedPathVec;
	}

	return std::vector<core::GameTile>();
}


void core::GameBoard::rotateToDestination(const MoveToAction* moveToCmd, Unit* unit)
{
	auto vertex = unit->getUnitVertexRotation();
	HexVertexNumber curVertex{ vertex.vertexNum + 5 }; // left vertex to cur
	GameTile currentPos = unit->getPosition();
	int distance = pathfinding::getDistance(currentPos, moveToCmd->m_destination);
	for (int i : views::iota(1, 7))
	{
		auto tilesInfrontVec = pathfinding::getLineOfSightWithoutBarriers(*this, currentPos, TileDistance{ (unsigned)distance + 5 }, curVertex);
		if (ranges::find(tilesInfrontVec, moveToCmd->m_destination) != end(tilesInfrontVec))
		{
			if (i > 3) // if unit rotated more than one vertex
				unit->rotateToVertex(curVertex);

			unit->setUnitVertexRotation(curVertex);
			return;
		}
		curVertex = HexVertexNumber{ curVertex.vertexNum + 1 };
	}
	
	// TODO could be good to do refactor here, cuz we shouldn't be there ,unit should be able to find his proper rotation
	// if we have  reached this part something is very wrong!
	// throw std::runtime_error("pzd\n");
}

std::vector<GameTile> core::GameBoard::getStraightLine(const GameTile& from, const GameTile& to)
{
	return pathfinding::drawLine(from, to);
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
