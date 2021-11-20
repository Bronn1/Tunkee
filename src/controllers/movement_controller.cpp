#include "movement_controller.h"
#include "../core/game_command.h"

controllers::MovementController::MovementController(core::GameRulesEngine* engine, const PlayerIdentifier player) :
	m_gameEngineRules(engine), m_player(player)
{

}

std::vector<core::GameTile> controllers::MovementController::selectUnit(const ObjectIdentifier& unitID) const
{
	return std::vector<core::GameTile>();
}

std::vector<core::GameTile> controllers::MovementController::moveUnit(const ObjectIdentifier& unitID, const core::GameTile& dest) const
{
	auto cmdMove{ std::make_shared<MoveToAction>() };
	cmdMove.get()->m_unitID = { unitID };
	cmdMove.get()->m_playerID = { 1 };
	cmdMove.get()->m_destination = dest;

	return m_gameEngineRules->executeMoveUnit(cmdMove.get());
}

MoveAreaAndFirstLayerSize controllers::MovementController::getMoveArea(const ObjectIdentifier& unitID) const
{
	auto cmdArea{ std::make_shared<GetMoveAreaQuery>() };
	cmdArea.get()->m_unitID = { unitID };
	cmdArea.get()->m_playerID = { 1 };

	return m_gameEngineRules->queryMoveArea(cmdArea.get());
}
