#pragma once

#include "../core/game_rules_engine.h"
#include "../core/data_types.h"
#include "../core/game_tile.h"

#include <vector>

namespace controllers {
	class MovementController
	{
	public:
		MovementController(core::GameRulesEngine* engine, const PlayerIdentifier player);
		std::vector<core::GameTile> selectUnit(const ObjectIdentifier& unitID) const;
		std::vector<core::GameTile> moveUnit(const ObjectIdentifier& unitID, const core::GameTile& dest) const;
		MoveAreaAndFirstLayerSize getMoveArea(const ObjectIdentifier& unitID) const;
	private:
		core::GameRulesEngine* m_gameEngineRules;
		PlayerIdentifier m_player;
	};
}

