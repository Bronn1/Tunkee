#include "game_engine.h"

core::GameEngine::GameEngine(const GameBoard& board, UnitManagerPtr unitMng, const PlayerIdentifier playerOneId, const PlayerIdentifier playerWTwoId)
    : m_board(board), m_damageCalculator(), m_unitManager(std::move(unitMng))
{
    m_gameRules = std::move(std::make_unique<GameRulesBasic>());
    m_gameRules->setUnitManager(m_unitManager.get());
    m_gameRules->setStage(GameRulesInterface::GameStage::Setup);
    m_damageCalculator.initProbabilityTables();
}

void core::GameEngine::moveUnit(MoveToAction* action)
{
    if (m_gameRules->isMoveActionAllowed(*action))
    {
        auto unit = m_unitManager->getUnitIfExist(action->m_unitID);
        GameTile startingPos = (*unit)->getPosition();
        auto  movePath = (*unit)->moveTo(action, m_board);
        auto moveInfo  = MoveUnitInfo(movePath, startingPos, (*unit)->getID(), (*unit)->getUnitVertexRotation());
        notifyHost(moveInfo);
        auto moveAreaQuery{ std::make_unique<GetMoveAreaQuery>(action->m_playerID, action->m_unitID) };
        auto moveArea = (*unit)->getMoveArea(m_board);
        notifyHost(moveArea);
    }
    else
    {
        std::cout << m_gameRules->getLastError() << "\n";
    }
}

void core::GameEngine::shootUnit(ShootAction* action)
{
    if (m_gameRules->isShootActionAllowed(*action))
    {
        // game rules already checked correctness of units
        auto sourceUnit = m_unitManager->getUnitIfExist(action->m_unitID);
        auto targetUnit = m_unitManager->getUnitIfExist(action->m_target);

        bool isUnitInLineOfSight = (*sourceUnit)->isTargetInLineOfSight(m_board, (*targetUnit)->getPosition());
        if (!isUnitInLineOfSight) return;
        auto lineOfFireVec = m_board.getStraightLine(sourceUnit.value()->getPosition(), targetUnit.value()->getPosition());
        auto unitShotInfo = m_damageCalculator.shot(sourceUnit.value(), targetUnit.value(), lineOfFireVec);
        if ((*targetUnit)->getOwnerID() != m_gameHost)
        {
            if (!(*targetUnit)->isDamageVisibleForEnemy(unitShotInfo.m_damageTypeName))
                unitShotInfo.m_damageTypeName = tankDamageSystem::kHiddenDamage;
        }
        notifyHost(unitShotInfo);
        auto moveArea = (*sourceUnit)->getMoveArea(m_board);
        notifyHost(moveArea);
        if (m_gameRules->isGameEndedFor(PointOfView::Enemy))
            std::cout << "Player: " << m_gameRules->getCurrentPlayer().identifier << " has won, Gratz!\n";

    }
    else
    {
        std::cout << m_gameRules->getLastError() << "\n";
    }

}

void core::GameEngine::finishSetupStage(FinishSetupStage* finishSetupStageAction)
{
    Player& player = (m_playerOne.getId() == finishSetupStageAction->m_playerID) ? m_playerOne : m_playerTwo;
    bool isAbleToEnd = player.endSetupStage(); // move to game_rules strategy
    // notifyHost or return some success indicator
    // when both players ready notifyHost about visible/created units for both
    //notifyHost(m_unitMng.getAllVisibleUnitsForPlaer());
    if (isAbleToEnd) {
        m_gameRules->setStage(GameRulesInterface::GameStage::ActionPhase);
        m_gameRules->setCurrentPlayer(m_playerOne.getId());
        m_gameRules->setActiveUnits(m_playerOne.getId());
        m_gameRules->setActiveUnits(m_playerTwo.getId());
    }
}

void core::GameEngine::finishActionPhase(FinishActionPhase* finishActionPhase)
{
    // TODO should we allow to let ppl finish stage with any  action left on unit??
    auto unit =  m_unitManager->getUnitIfExist(m_gameRules->getSelectedUnit());
    if (unit)
        (*unit)->setActionState(ActionStatus::empty);

    if (m_gameRules->isGameEndedFor(PointOfView::Enemy))
        std::cout << "Player: " << m_gameRules->getCurrentPlayer().identifier << " has won, Gratz!\n";
    // try to implement as a strategy
    m_gameRules->nextActionPhase(finishActionPhase);

    if (m_gameRules->getCurrentStage() == GameRulesInterface::GameStage::TurnEnd)
        endOfTurn();
}

void core::GameEngine::requestMoveArea(GetMoveAreaQuery* moveAreaQuery)
{
    auto unit = m_unitManager->getUnitIfExist(moveAreaQuery->m_unitID);
    if (unit)
    {
        auto moveArea = (*unit)->getMoveArea(m_board);
        notifyHost(moveArea);
    }
}

void core::GameEngine::selectUnit(const SelectUnitQuery* selectUnitQuery)
{
    auto selectedUnitNotify = UnitSelectedInfo(m_gameRules->selectUnit(selectUnitQuery));
    notifyHost(selectedUnitNotify);

    auto unit = m_unitManager->getUnitIfExist(selectedUnitNotify.m_unitId);
    if (unit)
    {
        //auto moveAreaQuery{ std::make_shared<GetMoveAreaQuery>(selectUnitQuery->m_playerID, selectUnitQuery->m_unitID) };
        auto moveArea = (*unit)->getMoveArea(m_board);
        notifyHost(moveArea);
    }
}

void core::GameEngine::createUnitStateMsg(const UnitStateQuery* unitStateQuery)
{
    auto selectedUnit = m_unitManager->getUnitIfExist(unitStateQuery->m_selectedUnit);
    auto targetUnit = m_unitManager->getUnitIfExist(unitStateQuery->m_targetUnit);
    if (!selectedUnit || !targetUnit) return;
 
    PointOfView pointOfView = (unitStateQuery->m_player == (*targetUnit)->getOwnerID()) ? PointOfView::Player : PointOfView::Enemy;
    auto unitStateMsg = GameMessageFactory::createUnitStateMsg(m_board, *selectedUnit, *targetUnit, m_damageCalculator,  unitStateQuery->m_requiredGunAngleToShot, pointOfView);
    notifyHost(unitStateMsg);
}

UnitIdentifier core::GameEngine::addNewUnit(std::unique_ptr<core::Unit> unit)
{
    Player& player = (m_playerOne.getId() == unit->getOwnerID()) ? m_playerOne : m_playerTwo;
    if (player.isAbleToAddUnit() && m_board.isAccessible(unit->getPosition())) {
        GameTile unitPosition = unit->getPosition();
        UnitIdentifier createdUnitId = m_unitManager->addUnit(std::move(unit));
        player.addUnit(createdUnitId);
        m_board.setTileAccessible(unitPosition, false);

        return createdUnitId;
    }
    return UnitIdentifier{ 0 };
}

void core::GameEngine::rotateUnitGun(RotateUnitActiom* rotateAction)
{
    // TODO should we check everytime if somebody trying to cheat????..(checks for current player and unit owner)
    auto unit = m_unitManager->getUnitIfExist(m_gameRules->getSelectedUnit());
    if (!unit) return; // just add logger (doubt we should throw here)
        
    (*unit)->setGunRotation(rotateAction->m_angle);
    RotateGunInfo gunRotation((*unit)->getGunRotation(), (*unit)->getID());
    notifyHost(gunRotation);
}

void core::GameEngine::endOfTurn()
{
    std::cout << "END OF TURN...\n";
    auto changedUnitStatesVec = m_damageCalculator.nextTurn();
    for (const auto& state : changedUnitStatesVec)
        notifyHost(state);
    m_unitManager->passNextTurnToUnits();
    //m_unitManager->setUnitsActions(ActionStatus::full);
    m_gameRules->setStage(GameRulesInterface::GameStage::ActionPhase);
    m_gameRules->setActiveUnits(m_playerOne.getId());
    m_gameRules->setActiveUnits(m_playerTwo.getId());
    m_gameRules->isGameEndedFor(PointOfView::Enemy);
    m_gameRules->isGameEndedFor(PointOfView::Player);
    //TODO show  winning or losing msg if game ended
}

void core::GameEngine::setPlayer(const Player& player)
{
    // TODO additional checker for full lobby
    if (m_playerOne.getId() == PlayerIdentifier{ 0 }) {
        m_playerOne = player;
    }
    else {
        m_playerTwo = player;
    }
    m_gameRules->setPlayer(player.getId());
}
