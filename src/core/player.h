#pragma once

#include "events.h"
#include "unit.h"
#include "data_types.h"
#include "game_tile.h"

#include <vector>
#include <map>

//using UnitPtr

namespace core {
	/** For now unused
	*/
	class Player : public events::Events
	{
	public:
		Player() = default;
		Player(const PlayerIdentifier id) : m_id(id) {}

		//void addUnit(Unit unit) { m_ownedUnits.insert({ unit.getID(), unit }); }
		//void moveUnitTo(const ObjectIdentifier& id, const GameTile& destination);
		
		//GameTile getUnitPosition(const ObjectIdentifier& id) const ;
		
		//inline bool hasUnitWithId(const ObjectIdentifier& id) const;
		//bool checkUnitPositionAndSetIfNot(const ObjectIdentifier& id, const GameTile& pos);
	private:
		const PlayerIdentifier m_id;
		//std::vector<Unit> m_ownedUnits;
		//std::map < ObjectIdentifier, Unit, Comparator<ObjectIdentifier>> m_ownedUnits;
	};
}
