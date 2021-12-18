#include "game_controller.h"
#include "../core/game_command.h"



controllers::GameController::GameController(core::GameEngine* engine, const PlayerIdentifier player) :
	m_gameEngine(engine), m_player(player)
{

}

void controllers::GameController::moveUnit(const UnitIdentifier unitID, const core::GameTile& dest) const
{
	auto cmdMove{ std::make_shared<MoveToAction>() };
	cmdMove.get()->m_unitID = { unitID };
	cmdMove.get()->m_playerID = { 1 };
	cmdMove.get()->m_destination = dest;
	m_gameEngine->moveUnit(cmdMove.get());
}


void controllers::GameController::onUnitClicked(const UnitIdentifier unitID) const
{
	auto selectUnitQuery{ std::make_shared<SelectUnitQuery>(m_player, unitID) };
	m_gameEngine->selectUnit(selectUnitQuery.get());
}

void controllers::GameController::finishSetupStage(PlayerIdentifier playerId)
{
	auto finishSetupStageAction{ std::make_shared<FinishSetupStage>() };
	finishSetupStageAction->m_playerID = playerId;
	m_gameEngine->finishSetupStage(finishSetupStageAction.get()); 
}

void controllers::GameController::finishActionPhase(PlayerIdentifier playerId)
{
	auto finishActionPhase{ std::make_shared<FinishActionPhase>() };
	finishActionPhase->m_playerID = playerId;
	m_gameEngine->finishActionPhase(finishActionPhase.get());
	
	// TODO for test
	auto selectUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{2}, UnitIdentifier{1}) };
	//m_gameEngine->selectUnit(selectUnitQuery.get());
	finishActionPhase->m_playerID = PlayerIdentifier{ 2 };
	m_gameEngine->finishActionPhase(finishActionPhase.get());
}

controllers::UnitSetupContoller::UnitSetupContoller(core::GameEngine* engine, const PlayerIdentifier player) :
	m_gameEngine(engine), m_player(player)
{
}

std::optional<SceneNodePtr> controllers::UnitSetupContoller::addUnit(const core::GameTile& position)
{
	auto tmpUnit = m_tankFactory.createBacisTank(position, m_player);
	UnitIdentifier unitId = (*tmpUnit).getID();
	UnitIdentifier addedUnitID = m_gameEngine->addNewUnit(std::move(tmpUnit));
	if (addedUnitID.identifier)
	{
		float rotation = 270.f;
		float scale = 0.17f;
		return { m_tankFactory.createBacisTankView(addedUnitID, rotation, scale) };
		//m_worldView->addNewUnitView(std::move(basicTankView), position);
	}
	return std::nullopt;
}
