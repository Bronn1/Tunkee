#pragma once

#include "events.h"
#include "i_engine.h"
#include "game_board.h"
#include "game_rules.h"
#include "damage_calculator.h"
#include "unit_manager.h"
#include "unit_logger.h"

#include <vector>
#include <memory>



namespace core {
constexpr int kHostID = 1;
class GameEngine final : public IEngine, public events::Events<IEngine>
{
public:
    //using ActionPtr = std::shared_ptr<GameAction>;
   // using QueryPtr = std::shared_ptr<GameQuery>;
    using UnitManagerPtr = std::unique_ptr<core::UnitManager>;
    using GameRulesCheckerPtr = std::unique_ptr<core::GameRulesInterface>;

    explicit GameEngine(const GameBoard& board, UnitManagerPtr unitMng, const PlayerIdentifier playerOneId = { 1 }, const PlayerIdentifier playerWTwoId = { 2 });

    void moveUnit(MoveToAction* action) override;
    void shootUnit(ShootAction* action) override;
    void requestMoveArea(GetMoveAreaQuery* moveAreaQuery) override;
    void selectUnit(const SelectUnitQuery* selectUnitQuery) override;
    void createUnitStateMsg(const UnitStateQuery* unitStateQuery) override;
    UnitIdentifier addNewUnit(std::unique_ptr<core::Unit> unit) override;
    void rotateUnitGun(RotateUnitActiom* rotateAction) override;

    void finishSetupStage(FinishSetupStage* finishSetupStageAction) override;
    void finishActionPhase(FinishActionPhase* finishActionPhase) override;
    std::vector<UnitIdentifier> getUnitIDsForPlayer(const PlayerIdentifier playerId) const  override { return m_unitManager->getActiveUnitsForPlayer(playerId); }

    //void undoLastCommand(const ActionPtr& cmd) override;
    void endOfTurn() override;
    //const PlayerIdentifier getCurrentPlayer() const { return m_playerOneId; }
    void setPlayer(const Player& player) override;
    //void getMoveAreaForUnit(const ActionPtr& cmd) { m_board.getMoveAreaForUnit(cmd); }

private:
private:
    GameBoard m_board;
    GameRulesCheckerPtr m_gameRules;
    Player m_playerOne{ NullPlayer{} };
    Player m_playerTwo{ NullPlayer{} };
    PlayerIdentifier m_gameHost{ kHostID };
    std::stack < ActionPtr > m_actionHistory;
    DamageCalculator m_damageCalculator;
    UnitManagerPtr  m_unitManager;
    UnitLogger      m_unitLogger;
    GameRulesBasic test;
};
} // namespace core
