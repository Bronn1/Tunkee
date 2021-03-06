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
    if (!m_gameRules->isMoveActionAllowed(*action)) {
        std::cerr << m_gameRules->getLastError() << "\n";
    }

    auto unit = m_unitManager->getUnitIfExist(action->m_unitID);
    GameTile startingPos = (*unit)->getPosition();
    auto  movePath = (*unit)->moveTo(action, m_board);
    auto moveInfo = MoveUnitInfo(movePath, startingPos, (*unit)->getID(), (*unit)->getUnitVertexRotation());
    m_unitLogger.addAction((*unit)->getID(), (*unit)->getMoveStatus());
    notifyHost(moveInfo);
    auto moveAreaQuery{ std::make_unique<GetMoveAreaQuery>(action->m_playerID, action->m_unitID) };
    auto moveArea = (*unit)->getMoveArea(m_board);
    notifyHost(moveArea); //  NOTify also Ai and second player if multiplayer
}

void core::GameEngine::shootUnit(ShootAction* action)
{
    if (!m_gameRules->isShootActionAllowed(*action)) {
        std::cerr << m_gameRules->getLastError() << "\n";
    }

    // game rules already checked correctness of units
    auto sourceUnit = m_unitManager->getUnitIfExist(action->m_unitID);
    auto targetUnit = m_unitManager->getUnitIfExist(action->m_target);

    bool isUnitInLineOfSight = (*sourceUnit)->isTargetInLineOfSight(m_board, (*targetUnit)->getPosition());
    if (!isUnitInLineOfSight) return;
    auto lineOfFireVec = m_board.getStraightLine(sourceUnit.value()->getPosition(), targetUnit.value()->getPosition());
    auto unitShotInfo = m_damageCalculator.shot(sourceUnit.value(), targetUnit.value(), lineOfFireVec);
    if ((*targetUnit)->getOwnerID() != m_gameHost) {
        if (!(*targetUnit)->isDamageVisibleForEnemy(unitShotInfo.m_damageTypeName))
            unitShotInfo.m_damageTypeName = tankDamageSystem::kHiddenDamage;
    }
    //m_unitLogger.addAction((*sourceUnit)->getID(), UnitAction::Shot);
    notifyHost(unitShotInfo);
    auto moveArea = (*sourceUnit)->getMoveArea(m_board);
    notifyHost(moveArea);
    if (m_gameRules->isGameEndedFor(PointOfView::Enemy))
        std::cout << "Player: " << m_gameRules->getCurrentPlayer().identifier << " has won, Gratz!\n";
}

void core::GameEngine::finishSetupStage(FinishSetupStage* finishSetupStageAction)
{
    Player& player = (m_playerOne.getId() == finishSetupStageAction->m_playerID) ? m_playerOne : m_playerTwo;
    bool isAbleToEnd = player.endSetupStage(); // move to game_rules strategy
    // notifyHost or return some success indicator
    // when both players ready notifyHost  nofity about visible enemy units 
    //notifyHost(m_unitMng.getAllVisibleUnitsForPlaer());
    if (!isAbleToEnd) {
        return;
    }

    m_gameRules->setStage(GameRulesInterface::GameStage::ActionPhase);
    m_gameRules->setCurrentPlayer(m_playerOne.getId());
    m_gameRules->setActiveUnits(m_playerOne.getId());
    m_gameRules->setActiveUnits(m_playerTwo.getId());

    m_unitLogger.initUnits(m_unitManager->getAllUnits());

}

void core::GameEngine::finishActionPhase(FinishActionPhase* finishActionPhase)
{
    if (m_gameRules->getCurrentStage() == GameRulesInterface::GameStage::Setup)
        return;
    auto unit =  m_unitManager->getUnitIfExist(m_gameRules->getSelectedUnit());
    if (unit) {
        (*unit)->setActionState(ActionStatus::empty);
    }
        
    if (m_gameRules->isGameEndedFor(PointOfView::Enemy)) {// notify gui
        std::cout << "Player: " << m_gameRules->getCurrentPlayer().identifier << " has won, Gratz!\n";
    }
    // try to implement as a strategy
    m_gameRules->nextActionPhase(finishActionPhase);

    if (m_gameRules->getCurrentStage() == GameRulesInterface::GameStage::TurnEnd)
        endOfTurn();
}


void core::GameEngine::requestMoveArea(GetMoveAreaQuery* moveAreaQuery)
{
    auto unit = m_unitManager->getUnitIfExist(moveAreaQuery->m_unitID);
    if (!unit){
        return;
    }
    auto moveArea = (*unit)->getMoveArea(m_board);
    notifyHost(moveArea);
}

void core::GameEngine::selectUnit(const SelectUnitQuery* selectUnitQuery)
{
    auto selectedUnitNotify = UnitSelectedInfo(m_gameRules->selectUnit(selectUnitQuery));
    notifyHost(selectedUnitNotify);

    auto unit = m_unitManager->getUnitIfExist(selectedUnitNotify.m_unitId);
    if (!unit) {
        return;
    }
    //auto moveAreaQuery{ std::make_shared<GetMoveAreaQuery>(selectUnitQuery->m_playerID, selectUnitQuery->m_unitID) };
    auto moveArea = (*unit)->getMoveArea(m_board);
    notifyHost(moveArea);
}

void core::GameEngine::createUnitStateMsg(const UnitStateQuery* unitStateQuery)
{
    auto selectedUnit = m_unitManager->getUnitIfExist(unitStateQuery->m_selectedUnit);
    auto targetUnit = m_unitManager->getUnitIfExist(unitStateQuery->m_targetUnit);
    if (!selectedUnit || !targetUnit) return;
 
    PointOfView pointOfView = (unitStateQuery->m_player == (*targetUnit)->getOwnerID()) ? PointOfView::Player : PointOfView::Enemy;
    //auto msgBuiler = GameMessageBuilder::createUnitStateMsg(m_board, *selectedUnit, *targetUnit, m_damageCalculator,  unitStateQuery->m_requiredGunAngleToShot, pointOfView);
    auto msgBuiler = GameMessageBuilder::createUnitStateMsg( *targetUnit, pointOfView).addMoveStatus(m_unitLogger.getLastActions((*targetUnit)->getID())).
                                          addEnemyInfo(m_board, *selectedUnit,  m_damageCalculator, unitStateQuery->m_requiredGunAngleToShot);

    notifyHost(msgBuiler.getStateMsg());
}

UnitIdentifier core::GameEngine::addNewUnit(std::unique_ptr<core::Unit> unit)
{
    Player& player = (m_playerOne.getId() == unit->getOwnerID()) ? m_playerOne : m_playerTwo;
    auto unitPosition = unit->getPosition();
    if (player.isAbleToAddUnit() && m_board.isAccessible(unitPosition) &&
        m_board.isInsideSetupArea(unit->getOwnerID(), unitPosition)) {
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
    auto unit = m_unitManager->getUnitIfExist(m_gameRules->getSelectedUnit());
    if (!unit) return; 
        
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
    m_unitLogger.endOfTurn();
    m_gameRules->isGameEndedFor(PointOfView::Enemy);
    m_gameRules->isGameEndedFor(PointOfView::Player);
    //TODO notify gui 
}

void core::GameEngine::setPlayer(const Player& player)
{
    if (m_playerOne.getId() == PlayerIdentifier{ 0 }) {
        m_playerOne = player;
    }
    else {
        m_playerTwo = player;
    }
    m_gameRules->setPlayer(player.getId());
}
