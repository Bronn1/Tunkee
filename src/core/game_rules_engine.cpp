#include "game_rules_engine.h"

core::GameRulesEngine::GameRulesEngine(const GameBoard& board, UnitManagerPtr unitMng, const PlayerIdentifier playerOneId, const PlayerIdentifier playerWTwoId)
    : m_board(board), m_damageCalculator(), m_playerOneId(playerOneId), m_playerTwoId(playerWTwoId), m_unitManager(unitMng)
{

}


/*void core::GameRulesEngine::executeAction(const ActionPtr& cmd)
{
	switch (cmd.get()->getType())
	{
	case ActionTypes::moveUnit:
		m_board.moveTo(static_cast<MoveToAction*>(cmd.get()));
		break;
	default:
		std::cerr << "Unknown action type is recivied:" << static_cast<int>(cmd.get()->getType()) << "\n";
		break;
	}
}
*/

std::vector<core::GameTile> core::GameRulesEngine::executeMoveUnit(MoveToAction* action)
{
	auto unit = (*m_unitManager).getUnitIfExist(action->m_unitID);
	if (unit)
		return m_board.moveTo(action, unit.value());
	else
		return std::vector<core::GameTile>();
}

MoveAreaAndFirstLayerSize core::GameRulesEngine::queryMoveArea(GetMoveAreaQuery* moveAreaQuery)
{
	auto unit = (*m_unitManager).getUnitIfExist(moveAreaQuery->m_unitID);
	if (unit)
		return m_board.getMoveAreaForUnit(moveAreaQuery, unit.value());
	else
		return MoveAreaAndFirstLayerSize{};
}
