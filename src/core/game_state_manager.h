#pragma once

#include "game_command.h"


using ActionPtr = std::shared_ptr<GameAction>;

namespace core
{
	/*
	*
	**/
	class GameStateManager
	{
	public:
		GameStateManager() {};
	private:
		std::stack < ActionPtr > m_actionHistory;
	};
}
