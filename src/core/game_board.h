#pragma once

#include "game_tile.h"
#include "unit.h"
#include "game_command.h"

#include <unordered_map>
#include <vector>
#include <memory>

using ActionPtr = std::shared_ptr<GameAction>;

namespace core {

	class GameBoard
	{
		
	public:
		GameBoard() = default;
		GameBoard(std::vector<GameTileType> tiles, int width, int height);

		MoveAreaInfo getMoveAreaForUnit(Unit *unit);
		std::vector<GameTile> moveTo(const MoveToAction* moveToCmd, Unit* unit);
		void rotateToDestination(const MoveToAction* moveToCmd, Unit* unit);
		std::vector<GameTile> getStraightLine(const GameTile & from, const GameTile & to);
		
		inline GameTile getTile(const  GameTile& tile) const {
			if (m_tiles.contains(tile)) return m_tiles.at(tile); 
			else return NullTile();
		}
		PlayerIdentifier getCurrentPlayerId() { return PlayerIdentifier{ 1 }; }
		const std::vector<GameTile>& getTileCoordinates() { return m_orderedBoard; }

		inline void setTileType(const GameTile& tile, const GameTileType type)
		{ if (m_tiles.contains(tile)) 
			m_tiles.at(tile).m_type = type; }

		inline void setTileAccessible(const GameTile& tile, bool isAccessible)
		{
			if (m_tiles.contains(tile))
				m_tiles.at(tile).isAccessible = isAccessible;
		}

		inline bool isAccessible(const GameTile& tile) const { return  m_tiles.at(tile).isAccessible; }
		inline bool contains(const GameTile& tile) const { return  m_tiles.contains(tile); }
		
		void printBoardTest();
		
		~GameBoard() = default;
	private:
		int m_height{0};
		int m_width{ 0 };
		/*! mostly for pathfinding purposes */
		std::unordered_map<GameTile, GameTile> m_tiles{};
		/*! for visual board representaion*/
		std::vector<GameTile> m_orderedBoard{};
	};
}
