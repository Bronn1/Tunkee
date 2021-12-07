#pragma once

#include "game_board.h"
#include "game_command.h"
#include "game_rules.h"
#include "damage_calculator.h"
#include "unit_manager.h"
#include "events.h"

#include <vector>
#include <memory>

using ActionPtr = std::shared_ptr<GameAction>;
using QueryPtr = std::shared_ptr<GameQuery>;
using UnitManagerPtr = std::unique_ptr<core::UnitManager>;
using GameStateCheckerPtr = std::unique_ptr<core::GameRulesInterface>;

namespace core {

	class GameEngine : public events::Events<GameEngine>
	{
	public:
		explicit GameEngine(const GameBoard& board, UnitManagerPtr unitMng, const PlayerIdentifier playerOneId = {1 }, const PlayerIdentifier playerWTwoId = {2 });
		
		void moveUnit(MoveToAction* action);
		void switchToNewStage(FinishSetupStageAction* finishSetupStageAction);
		void requestMoveArea(GetMoveAreaQuery* moveAreaQuery);
		void selectUnit(const SelectUnitQuery* selectUnitQuery);
		UnitIdentifier addNewUnit(std::unique_ptr<core::Unit> unit);

		void undoLastCommand(const ActionPtr& cmd);
		void endOfTurn();
		//const PlayerIdentifier getCurrentPlayer() const { return m_playerOneId; }
		void setPlayer(const Player& player);
		//void getMoveAreaForUnit(const ActionPtr& cmd) { m_board.getMoveAreaForUnit(cmd); }

	private:
	private:
		GameBoard m_board;
		GameStateCheckerPtr m_gameRulesChecker;
		Player playerOne{ NullPlayer{} };
		Player playerTwo{ NullPlayer{} };
		std::stack < ActionPtr > m_actionHistory;
		DamageCalculator m_damageCalculator;
		UnitManagerPtr  m_unitManager;
	};
}