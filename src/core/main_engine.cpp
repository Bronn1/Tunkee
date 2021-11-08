#include "main_engine.h"

core::MainEngine::MainEngine(const GameBoard& board, UnitManagerPtr unitMng, const PlayerIdentifier playerOneId, const PlayerIdentifier playerWTwoId)
    : m_board(board), m_damageCalculator(unitMng), m_playerOneId(playerOneId), m_playerTwoId(playerWTwoId)
{
	m_board.setUnitManager(unitMng);
}


void core::MainEngine::executeAction(const ActionPtr& cmd)
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

void core::MainEngine::executeQuery(const QueryPtr& query)
{
	switch (query.get()->getType())
	{
	case QueryTypes::getMoveArea:
		m_board.getMoveAreaForUnit(static_cast<GetMoveAreaQuery*>(query.get()));
		break;
	default:
		std::cerr << "Unknown query type is recivied:" << static_cast<int>(query.get()->getType()) << "\n";
		break;
	}
}
