#pragma once

#include "game_board.h"
#include "game_command.h"
#include "game_state_manager.h"
#include "damage_calculator.h"

#include <vector>
#include <memory>

using ActionPtr = std::shared_ptr<GameAction>;
using QueryPtr = std::shared_ptr<GameQuery>;
using UnitManagerPtr = std::shared_ptr<core::UnitManager>;

namespace core {
	class MainEngine
	{
	public:
		explicit MainEngine(const GameBoard& board, UnitManagerPtr unitMng, const PlayerIdentifier playerOneId = { 1 }, const PlayerIdentifier playerWTwoId = { 2 });

		void executeAction(const ActionPtr& cmd);
		void executeQuery(const QueryPtr& query);
		void undoLastCommand(const ActionPtr& cmd);
		void endOfTurn();
		const PlayerIdentifier getCurrentPlayer() const { return m_playerOneId; }
		//void getMoveAreaForUnit(const ActionPtr& cmd) { m_board.getMoveAreaForUnit(cmd); }


	private:
		GameBoard m_board;
		GameStateManager m_gameStateManager;
		PlayerIdentifier m_playerOneId;
		PlayerIdentifier m_playerTwoId;
		ActionStateStatus  m_playerRemainingActivity{ ActionStateStatus::full };
		std::stack < ActionPtr > m_actionHistory;
		DamageCalculator m_damageCalculator;
	};
}