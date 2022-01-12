#include "game_controller.h"
#include "../core/game_command.h"



controllers::GameController::GameController(core::GameEngine* engine, const PlayerIdentifier player) :
	m_gameEngine(engine), m_player(player)
{

}

void controllers::GameController::moveUnit(const UnitIdentifier unitID, const core::GameTile& dest) const
{
	auto cmdMove{ std::make_shared<MoveToAction>() };
	cmdMove->m_unitID = { unitID };
	cmdMove->m_playerID = m_player;
	cmdMove->m_destination = dest;
	m_gameEngine->moveUnit(cmdMove.get());
}

void controllers::GameController::onChangeUnitRotation(const UnitIdentifier unitID, const Angle rotation, const SetUnitRotation::Type rotationType)
{
	auto rotateAction{ std::make_unique<SetUnitRotation>(rotation, rotationType) };
	rotateAction->m_unitID = { unitID };
	rotateAction->m_playerID = m_player;
	m_gameEngine->setRotation(rotateAction.get());
}

void controllers::GameController::onUnitClicked(const UnitIdentifier selectedUnitID, const UnitIdentifier clickedUnitID) const
{
	
	if (m_ownUnits.contains(clickedUnitID)) {
		auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(m_player, clickedUnitID) };
		m_gameEngine->selectUnit(selectUnitQuery.get());
	}
	else if (selectedUnitID) {
		auto shootAction { std::make_unique<ShootAction>(m_player, selectedUnitID, clickedUnitID)};
		m_gameEngine->shootUnit(shootAction.get());
	}
}

void controllers::GameController::finishSetupStage(PlayerIdentifier playerId)
{
	auto finishSetupStageAction{ std::make_shared<FinishSetupStage>() };
	finishSetupStageAction->m_playerID = playerId;
	m_gameEngine->finishSetupStage(finishSetupStageAction.get()); 
	for (auto& id : m_gameEngine->getUnitIDsForPlayer(m_player))
		m_ownUnits.insert(id);
}

void controllers::GameController::finishActionPhase(PlayerIdentifier playerId)
{
	auto finishActionPhase{ std::make_shared<FinishActionPhase>() };
	finishActionPhase->m_playerID = playerId;
	m_gameEngine->finishActionPhase(finishActionPhase.get());
	
	// TODO for test
	//auto selectUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{2}, UnitIdentifier{1}) };
	//m_gameEngine->selectUnit(selectUnitQuery.get());
	finishActionPhase->m_playerID = PlayerIdentifier{ 2 };
	m_gameEngine->finishActionPhase(finishActionPhase.get());
}

controllers::UnitSetupContoller::UnitSetupContoller(core::GameEngine* engine, const PlayerIdentifier player) :
	m_gameEngine(engine), m_player(player)
{
}

std::optional<UnitViewPtr> controllers::UnitSetupContoller::addUnit(const core::GameTile& position)
{
	Angle rotation{ 270.f };
	float scale = 0.17f;
	auto tmpUnit = m_tankFactory.createBacisTank(position, rotation, m_player);
	UnitIdentifier addedUnitID = m_gameEngine->addNewUnit(std::move(tmpUnit));
	if (addedUnitID)
	{
		return { m_tankFactory.createBacisTankView(addedUnitID, rotation, scale) };
		//m_worldView->addNewUnitView(std::move(basicTankView), position);
	}
	return std::nullopt;
}
