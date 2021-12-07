#include "game_rules.h"

bool core::GameRulesBasic::isMoveActionAllowed(const MoveToAction& moveAction) 
{
	if (m_currStage != GameStage::PlayerIdle && m_currStage != GameStage::ActionPhase)
		return false;

	// checking nobody mocking us with wrong requests through connection(maybe unnecessary)
	if (moveAction.m_playerID == m_currentPlayer && m_selectedUnit == moveAction.m_unitID)
	{
		auto unitPtr = m_unitManager->getUnitIfExist(m_selectedUnit);
		if (unitPtr)
		{
			bool canMove = unitPtr.value()->canMove();
			m_lastError = (canMove) ? "" : "Unit doesn't have unit activity left!";
			return canMove;
		}
		//
	}
	else
		m_lastError = "Already another unit is selected\n";
	return false;
}

UnitIdentifier core::GameRulesBasic::selectUnit(const SelectUnitQuery* selectUnitQuery)
{
	if (m_currStage != GameStage::PlayerIdle && m_currStage != GameStage::EnemyTurn)
		return UnitIdentifier{ 0 };

	auto queryUnit = m_unitManager->getUnitIfExist(selectUnitQuery->m_unitID);
	if(!queryUnit || queryUnit.value()->getOwnerID() != m_currentPlayer)
		return m_selectedUnit;

	// if somebody trying to cheat
	if (selectUnitQuery->m_playerID != m_currentPlayer) {
		return m_selectedUnit;
	}

	// if player didnt pick any unit yet
	if (m_selectedUnit == UnitIdentifier{ 0 }) {
		m_selectedUnit = selectUnitQuery->m_unitID;
		return m_selectedUnit;
	}

	auto unitPtr = m_unitManager->getUnitIfExist(m_selectedUnit);
	if (unitPtr)
	{
		bool isUnitFull = unitPtr.value()->isUnitHaveFullActionState();

		if (m_selectedUnit != selectUnitQuery->m_unitID) {
			m_selectedUnit = (isUnitFull) ? selectUnitQuery->m_unitID : m_selectedUnit;
			return m_selectedUnit;
		}
		else { //diselect unit
			m_selectedUnit = (isUnitFull) ? UnitIdentifier{ 0 } : m_selectedUnit;
			return m_selectedUnit;
		}
	}

	return m_selectedUnit;
}

core::GameRulesInterface::GameRulesInterface()
{
}
