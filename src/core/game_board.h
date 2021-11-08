#pragma once

#include "events.h"
#include "game_tile.h"
//#include "player.h"
#include "unit.h"
#include "game_command.h"
#include "unit_manager.h"

#include <unordered_map>
#include <vector>
#include <memory>

//using PlayerPtr = std::shared_ptr<Player>;
using ActionPtr = std::shared_ptr<GameAction>;
using UnitPtr = std::shared_ptr<core::Unit>;
using UnitManagerPtr = std::shared_ptr<core::UnitManager>;

namespace core {

	class GameBoard : public events::Events
	{
		
	public:
		GameBoard() = default;
		GameBoard(std::vector<GameTileType> tiles, int width, int height);

		void getMoveAreaForUnit(const GetMoveAreaQuery* getAreaCmd);
		bool moveTo(const MoveToAction* moveToCmd);	
		
		inline GameTile getTile(const  GameTile& tile) const  { if (m_tiles.contains(tile)) return m_tiles.at(tile); }
		PlayerIdentifier getCurrentPlayerId() { return PlayerIdentifier{ 1 }; }
		//PlayerPtr& getCurrentPlayer() { return m_playerOne; }
		// prob better return const iterator
		const std::vector<GameTile>& getTileCoordinates() { return m_orderedBoard; }


		// TODO change some set methods, for test only rn
		//void setPlayerOne(PlayerPtr player) { m_playerOne = player; }
		inline void setTileType(const GameTile& tile, const GameTileType type)
		{ if (m_tiles.contains(tile)) 
			m_tiles.at(tile).m_type = type; }

		inline void setTileAccessible(const GameTile& tile, bool isAccessible)
		{
			if (m_tiles.contains(tile))
				m_tiles.at(tile).isAccessible = isAccessible;
		}
		inline void setUnitManager(const UnitManagerPtr unitMnr) { m_unitManager = unitMnr;  }

		inline bool isAccessible(const GameTile& tile) const { return  tile.isAccessible; }
		inline bool contains(const GameTile& tile) const { return  m_tiles.contains(tile); }
		
		void printBoardTest();
		
		~GameBoard() {
			//printBoardTest();
		}
	private:
		int m_height{0};
		int m_width{ 0 };
		/*! mostly for pathfinding purposes */
		std::unordered_map<GameTile, GameTile> m_tiles{};
		/*! for visual board representaion*/
		std::vector<GameTile> m_orderedBoard{};
		UnitManagerPtr m_unitManager;
		//PlayerPtr m_playerOne;
		//PlayerPtr m_playerTwo;
	};
}
