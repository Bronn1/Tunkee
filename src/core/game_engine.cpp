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
	auto unit = m_unitManager->getUnitIfExist(action->m_unitID);
	if (m_gameRules->isMoveActionAllowed(*action))
	{
		auto  movePath =  m_board.moveTo(action, unit.value());
		auto moveInfo  = MoveUnitInfo(movePath, unit.value()->getID());
		notify(moveInfo);
		auto moveAreaQuery{ std::make_shared<GetMoveAreaQuery>(action->m_playerID, action->m_unitID) };
		auto moveArea = m_board.getMoveAreaForUnit(unit.value());
		notify(moveArea);
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
		auto lineOfFireVec = m_board.getStraightLine(sourceUnit.value()->getPosition(), targetUnit.value()->getPosition());
		m_damageCalculator.shot(sourceUnit.value(), targetUnit.value(), lineOfFireVec);
	}
	else
	{
		std::cout << m_gameRules->getLastError() << "\n";
	}

}

void core::GameEngine::finishSetupStage(FinishSetupStage* finishSetupStageAction)
{
	Player& player = (m_playerOne.getId() == finishSetupStageAction->m_playerID) ? m_playerOne : m_playerTwo;
	bool isAbleToEnd = player.endSetupStage();
	// notify or return some success indicator
	// when both players ready notify about visible/created units for both
	//notify(m_unitMng.getAllVisibleUnitsForPlaer());
	if (isAbleToEnd) {
		m_gameRules->setStage(GameRulesInterface::GameStage::ActionPhase);
		m_gameRules->setCurrentPlayer(m_playerOne.getId());
		m_gameRules->setActiveUnits(m_playerOne.getId());
		m_gameRules->setActiveUnits(m_playerTwo.getId());
	}
}

void core::GameEngine::finishActionPhase(FinishActionPhase* finishActionPhase)
{
	m_gameRules->nextActionPhase(finishActionPhase);
	// TODO should we allow to let ppl finish stage with any  action left on unit??
	auto unit =  m_unitManager->getUnitIfExist(m_gameRules->getSelectedUnit());
	if (unit)
		unit.value()->setActionState(ActionStateStatus::empty);

	if (m_gameRules->getCurrentStage() == GameRulesInterface::GameStage::TurnEnd)
		endOfTurn();
}

void core::GameEngine::requestMoveArea(GetMoveAreaQuery* moveAreaQuery)
{
	auto unit = m_unitManager->getUnitIfExist(moveAreaQuery->m_unitID);
	if (unit)
	{
		auto moveArea = m_board.getMoveAreaForUnit(unit.value());
		notify(moveArea);
	}
}

void core::GameEngine::selectUnit(const SelectUnitQuery* selectUnitQuery)
{
	auto selectedUnitNotify = UnitSelectedInfo(m_gameRules->selectUnit(selectUnitQuery));
	notify(selectedUnitNotify);

	auto unit = m_unitManager->getUnitIfExist(selectedUnitNotify.m_unitId);
	if (unit)
	{
		auto moveAreaQuery{ std::make_shared<GetMoveAreaQuery>(selectUnitQuery->m_playerID, selectUnitQuery->m_unitID) };
		auto moveArea = m_board.getMoveAreaForUnit(unit.value());
		notify(moveArea);
	}
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

void core::GameEngine::setRotation(SetUnitRotation* rotateAction)
{
	// TODO should we check everytime if somebody trying to cheat????..(checks for current player and unit owner)
	auto unit = m_unitManager->getUnitIfExist(m_gameRules->getSelectedUnit());
	if (unit)
		(rotateAction->m_type == SetUnitRotation::Type::Body) ? unit.value()->setUnitRotation(rotateAction->m_angle) : 
			                                                    unit.value()->setGunRotation(rotateAction->m_angle);
}

void core::GameEngine::endOfTurn()
{
	std::cout << "END OF TURN...\n";
	m_unitManager->calculateAliveUnitsOnNextTurn();
	m_unitManager->setUnitsActions(ActionStateStatus::full);
	m_gameRules->setStage(GameRulesInterface::GameStage::ActionPhase);
	m_gameRules->setActiveUnits(m_playerOne.getId());
	m_gameRules->setActiveUnits(m_playerTwo.getId());
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
