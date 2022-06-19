#pragma once

#include "game_command.h"
#include "unit_manager.h"
#include "player.h"


namespace core
{
    /**
     * @brief TODO REFACTOR TO PURE INTERFACE and it will be strategy pattern in main engine 
     Prob we want to change some base rules  by player choice, so we need a base class for rules
    * and pick needed rules version of the game during game initialization
    */
    class GameRulesInterface
    {
    public:
        enum class GameStage
        {
            Setup,
            ActionPhase, 
            TurnEnd,
            GameEnd
        };
        using enum GameStage;
        //template<typename Action_t>
        virtual bool isMoveActionAllowed(const MoveToAction& moveToaction) = 0;
        virtual bool isShootActionAllowed(const ShootAction& action) = 0;
        //virtual bool isAllowedToEndTurn(const MoveToAction& moveToaction) = 0;
        virtual UnitIdentifier selectUnit(const SelectUnitQuery* selectUnitQuery) = 0;
        virtual void nextActionPhase(const FinishActionPhase* finishActionPhase) = 0;
        virtual bool isGameEndedFor(PointOfView pov) = 0;
        virtual void setActiveUnits(const PlayerIdentifier playerId) = 0;
        virtual void setPlayer(const PlayerIdentifier playerId) = 0;
        //virtual void startNewTurn() = 0;

        virtual ~GameRulesInterface() = default;

        std::string_view getLastError() const { return m_lastError; }
        void setUnitManager(UnitManager* manager) { m_unitManager = manager;  }
        void setCurrentPlayer(const PlayerIdentifier playerId) { m_currentPlayer = playerId; }
        
        void setStage(const GameStage state) { m_currStage = state; }

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
       
    };

    /* In first version of the rules players move units in turns one by one until all of units run out 
    * of possible actions. Player can not select another unit if he already started action with 
    * one unit until all possible actions has been used.
    **/
    class GameRulesBasic final : public  GameRulesInterface
    {
    public:
        GameRulesBasic() {};
        bool isMoveActionAllowed(const MoveToAction& moveToaction)  override;
        bool isShootActionAllowed(const ShootAction& shootAction) override;
        void nextActionPhase(const FinishActionPhase* finishActionPhase) override;
        UnitIdentifier selectUnit(const SelectUnitQuery* selectUnitQuery)  override ;
        bool isGameEndedFor(PointOfView pov)  override;
        void setActiveUnits(const PlayerIdentifier playerId) override;
        void setPlayer(const PlayerIdentifier playerId) override;
    private:
        void swapPlayers() { std::swap(m_currentPlayer, m_secondPlayer); }
        //ActionStatus  m_playerRemainingActivity{ ActionStatus::full };
        
        PlayerIdentifier m_secondPlayer{ 0 };
        const UnitIdentifier m_emptyUnitId{ 0 };
    };
}
