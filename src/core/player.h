#pragma once

#include "data_types.h"

#include <vector>
#include <set>

//using UnitPtr

namespace core {
	enum class UnitsPickType
	{
		gold,
		unitsNumber
	};

	/** Class defining a player. Before game start player has either gold to buy units or 
	* number of different units he can pick
	* Each player has owned units and  id
	*/
	class Player
	{
	public:
		Player(const PlayerIdentifier id, UnitsPickType pickType = UnitsPickType::unitsNumber);
		Player( const Player& other) = default;
		Player& operator =(const Player&) = default;

		void addUnit(UnitIdentifier id) { m_ownedUnitIds.insert(id); }
		void removeUnit(UnitIdentifier id) { m_ownedUnitIds.erase(id); }
		//TODO pass amount of gold or unit type
		bool isAbleToAddUnit();
		bool isFinishedPickingStage() const { return m_isFinishedPickingStage; }
		bool endPickingStage();
		PlayerIdentifier getId() const { return m_id; }
		//void moveUnitTo(const UnitIdentifier& id, const GameTile& destination);
		
		//GameTile getUnitPosition(const UnitIdentifier& id) const ;
		
		//inline bool hasUnitWithId(const UnitIdentifier& id) const;
		//bool checkUnitPositionAndSetIfNot(const UnitIdentifier& id, const GameTile& pos);
	private:
		PlayerIdentifier m_id;
		UnitsPickType m_pickType;
		bool m_isFinishedPickingStage{ false };
		//std::vector<Unit> m_ownedUnits;
		std::set < UnitIdentifier, Comparator<UnitIdentifier>> m_ownedUnitIds{};
	};

	class NullPlayer : public Player
	{
	public:
		NullPlayer(PlayerIdentifier id = { 0 }) : Player(id) {}
	};
}
