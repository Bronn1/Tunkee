#pragma once

#include "game_board.h" 
#include "data_types.h"

using namespace core;


// TODO separate class prob
namespace pathfinding {
	int getLength(const core::GameTile& tile);
	int getDistance(const core::GameTile& a, const core::GameTile& b);
	std::vector<GameTile> getNeighbors(const GameBoard& board, const GameTile& source);
	std::vector<GameTile> getAvailableArea(const GameBoard& board, const GameTile& source, const TileDistance movement);
	std::vector<GameTile> getShortestPath(const GameBoard& board, const GameTile& source, const GameTile& destination);
	std::vector<GameTile> _createOrderedPath(const std::unordered_map<GameTile, GameTile>&, const GameTile& source, const GameTile& destination);
	GameTile _roundToTile(const float q, const float r, const float s);
	// linear interpolation
	inline float _lerp(const float a, const float b, const float t);
	std::tuple<float, float, float> _cubeLerp(const GameTile& a, const GameTile& b, const float t);
	std::vector<GameTile> drawLine(const GameTile& source, const GameTile& destination);
}