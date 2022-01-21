#pragma once

#include "game_board.h" 
#include "data_types.h"

using namespace core;


// TODO separate class prob
namespace pathfinding {
	int getLength(const core::GameTile& tile);
	int getDistance(const core::GameTile& a, const core::GameTile& b);
	std::vector<GameTile> getNeighbors(const GameBoard& board, const GameTile& startingPos);
	std::vector<GameTile> getAvailableArea(const GameBoard& board, const GameTile& startingPos, const TileDistance movement);
	/**
	 * @brief Unit movement area should also count rotation that unit has to do to move to that tile. For now +1 movement point for rotation on any angle except neighbour vertices
	 * @param board 
	 * @param startingPos -  unit starting position
	 * @param movement 
	 * @param unitRotation - Hexagon vertex on which unit currently points
	 * @return 
	*/
	std::vector<GameTile> getAvailableAreaWithRotation(const GameBoard& board, const GameTile& startingPos, const TileDistance movement, HexVertexNumber unitRotation);
	std::vector<GameTile> getShortestPath(const GameBoard& board, const GameTile& source, const GameTile& destination);
	std::vector<GameTile> _createOrderedPath(const std::unordered_map<GameTile, GameTile>&, const GameTile& startingPos, const GameTile& destination);
	GameTile _roundToTile(const float q, const float r, const float s);
	// linear interpolation
	inline float _lerp(const float a, const float b, const float t);
	std::tuple<float, float, float> _cubeLerp(const GameTile& a, const GameTile& b, const float t);
	std::vector<GameTile> drawLine(const GameTile& source, const GameTile& destination);
	std::vector<GameTile> getLineOfSightWithoutBarriers(const GameBoard& board, const GameTile& startingPos, const TileDistance layersAmount, HexVertexNumber point);
	std::tuple<int, int, int, int> _getLineOfSightLayerCoordinates(HexVertexNumber point, int layer);
}