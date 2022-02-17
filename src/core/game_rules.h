#pragma once

#include "game_command.h"
#include "unit_manager.h"
#include "player.h"

using ActionPtr = std::shared_ptr<GameAction>;

namespace core
{
	/** Prob we want to change some base rules  by player choice, so we need a base class for rules
	* and pick needed rules version of the game during game initialization
	*/
	class GameRulesInterface
	{
	public:
		enum class GameStage
		{
			Setup,
			ActionPhase, 
			TurnEnd
		};
		using enum GameStage;
		GameRulesInterface();
		//template<typename Action_t>
		virtual bool isMoveActionAllowed(const MoveToAction& moveToaction) = 0;
		virtual bool isShootActionAllowed(const ShootAction& action) = 0;
		//virtual bool isAllowedToEndTurn(const MoveToAction& moveToaction) = 0;
		virtual UnitIdentifier selectUnit(const SelectUnitQuery* selectUnitQuery) = 0;
		virtual void nextActionPhase(const FinishActionPhase* finishActionPhase) = 0;
		//virtual void startNewTurn() = 0;

		virtual ~GameRulesInterface() = default;

		std::string_view getLastError() const { return m_lastError; }
		void setUnitManager(UnitManager* manager) { m_unitManager = manager;  }
		void setCurrentPlayer(const PlayerIdentifier playerId) { m_currentPlayer = playerId; }
		void setPlayer(const PlayerIdentifier playerId);
		void setStage(const GameStage state) { m_currStage = state; }
		void setActiveUnits(const PlayerIdentifier playerId);

		UnitIdentifier getSelectedUnit() const { return m_selectedUnit; }
		PlayerIdentifier getCurrentPlayer() const { return m_currentPlayer; }
		GameStage getCurrentStage() const { return m_currStage; }
	protected:
		GameStage m_currStage{ GameStage::Setup };
		UnitIdentifier m_selectedUnit{ 0 };
		PlayerIdentifier m_currentPlayer{ 0 };
		std::map< PlayerIdentifier, int, Comparator<PlayerIdentifier>> m_playerActiveUnits{};
		UnitManager* m_unitManager;
		std::string  m_lastError;
		const UnitIdentifier m_emptyUnitId{ 0 };
	};

	/* In first version of the rules players move units in turns one by one until all of units run out 
	* of possible actions. Player can not select another unit if he already started action with 
	* one unit until all possible actions has been used.
	**/
	class GameRulesBasic : public GameRulesInterface
	{
	public:
		GameRulesBasic() {};
		bool isMoveActionAllowed(const MoveToAction& moveToaction)  override;
		bool isShootActionAllowed(const ShootAction& shootAction) override;
		void nextActionPhase(const FinishActionPhase* finishActionPhase) override;
		UnitIdentifier selectUnit(const SelectUnitQuery* selectUnitQuery)  override final;

	private:
		
		//ActionStatus  m_playerRemainingActivity{ ActionStatus::full };


	};
}
