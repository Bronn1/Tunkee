#include "game_rules.h"
#include "exceptions.h"

bool core::GameRulesBasic::isMoveActionAllowed(const MoveToAction& moveAction)
{
    if (m_currStage != ActionPhase)
        return false;

    // checking nobody mocking us with wrong requests through connection(maybe unnecessary)
    if (moveAction.m_playerID == m_currentPlayer && m_selectedUnit == moveAction.m_unitID)
    {
        auto unitPtr = m_unitManager->getUnitIfExist(m_selectedUnit);
        if (unitPtr)
        {
            bool canMove = unitPtr.value()->canMove();
            m_lastError = (canMove) ? "" : "Unit doesn't have any activity left!";
            return canMove;
        }
        //
    }
    else
        m_lastError = "Movement is not allowed because of incorrect selected unit or enemy turn\n";
    return false;
}

void core::GameRulesBasic::nextActionPhase(const FinishActionPhase* finishActionPhase)
{
    assert(size(m_playerActiveUnits) == 2);

    if (finishActionPhase->m_playerID != m_currentPlayer ||
        m_currStage == TurnEnd || m_currStage == GameEnd)
        return;

    //auto curPlayer = m_playerActiveUnits.extract(m_currentPlayer);
    m_playerActiveUnits[m_currentPlayer]--;

    if (m_playerActiveUnits[m_secondPlayer] > 0)
    {
        m_currStage = ActionPhase;
        m_selectedUnit = m_emptyUnitId;
        swapPlayers();
    }
    else if (m_playerActiveUnits[m_currentPlayer] > 0)
    {
        //set current unit zero in action state
        m_currStage = ActionPhase;
        m_selectedUnit = m_emptyUnitId;
    }
    else
    {
        m_selectedUnit = m_emptyUnitId;
        m_currStage = TurnEnd;
        swapPlayers();
    }
}


UnitIdentifier core::GameRulesBasic::selectUnit(const SelectUnitQuery* selectUnitQuery)
{
    if (m_currStage != GameStage::ActionPhase)
        return m_emptyUnitId;

    auto queryUnit = m_unitManager->getUnitIfExist(selectUnitQuery->m_unitID);
    if (!queryUnit || (*queryUnit)->getOwnerID() != m_currentPlayer || 
        !bool(queryUnit.value()->isUnitHaveFullActionState()))
        return m_selectedUnit;

    // if somebody trying to cheat
    if (selectUnitQuery->m_playerID != m_currentPlayer) {
        return m_selectedUnit;
    }

    // TODO add check for unit owner


    // if player didnt pick any unit yet
    if (m_selectedUnit == m_emptyUnitId) {
        m_selectedUnit = selectUnitQuery->m_unitID;
        return m_selectedUnit;
    }

    auto unitPtr = m_unitManager->getUnitIfExist(m_selectedUnit);
    if (unitPtr)
    {
        bool isUnitFull = bool((*unitPtr)->isUnitHaveFullActionState());

        if (m_selectedUnit != selectUnitQuery->m_unitID) {
            m_selectedUnit = (isUnitFull) ? selectUnitQuery->m_unitID : m_selectedUnit;
            return m_selectedUnit;
        }
        else { //diselect unit
            m_selectedUnit = (isUnitFull) ? m_emptyUnitId : m_selectedUnit;
            return m_selectedUnit;
        }
    }

    return m_selectedUnit;
}

bool core::GameRulesBasic::isGameEndedFor(PointOfView pov) 
{
    if (m_unitManager->countAliveUnits((pov == PointOfView::Player) ? m_currentPlayer : m_secondPlayer)) 
        return false;

    m_currStage = GameEnd;
    m_unitManager->setAllDamageVisible();
    return true;
}

void core::GameRulesBasic::setActiveUnits(const PlayerIdentifier playerId)
{
    if (!m_playerActiveUnits.contains(playerId)) {
        throw InvalidPlayerIDException("Unknown players with ID: " + std::to_string(playerId.identifier));
    }

    m_playerActiveUnits[playerId] = m_unitManager->countActiveUnitsOwnedBy(playerId);
}

void core::GameRulesBasic::setPlayer(const PlayerIdentifier playerId)
{

    m_playerActiveUnits.insert({ playerId , 0 });
    if (m_currentPlayer == PlayerIdentifier{ 0 }) m_currentPlayer = playerId;
    else m_secondPlayer = playerId;
}

bool core::GameRulesBasic::isShootActionAllowed(const ShootAction& shootAction)
{
    if (m_currStage != ActionPhase)
        return false;

    if (shootAction.m_playerID == m_currentPlayer && m_selectedUnit == shootAction.m_unitID)
    {
        auto sourceUnit = m_unitManager->getUnitIfExist(m_selectedUnit);
        auto targetUnit = m_unitManager->getUnitIfExist(shootAction.m_target);
        
        if (sourceUnit && targetUnit && (*targetUnit)->getOwnerID() != m_currentPlayer)
        {
            bool canShoot = sourceUnit.value()->canShoot();
            m_lastError = (canShoot) ? "" : "Unit doesn't have activity to perform shooting!";
            return canShoot;
        }
    }
    else
        m_lastError = "Shooting is not allowed because of incorrect selected unit or enemy turn\n";
    return false;

}

