#pragma once

#include "data_types.h"
#include "events.h"
#include "game_tile.h"

namespace core 
{
	/** Contains logic of possible action unit can do in
	* a current turn. Unit has two possible types  of action
	* move and shot, so at every turn unit can do either move-move
	* or move-shot or shot-shot. Odd numbers in move and shot in a half size
	* rounded to the next up
	*/
	class UnitActionState
	{
	public:
		UnitActionState(const  TileDistance movePts, const Shots fireRate)
			: m_movePoints(movePts), m_rateOfFire(fireRate), m_remainingMovePoints(movePts), m_remainingShots(fireRate) {}
		void setState(const ActionStateStatus& state);
		TileDistance getRemainingMovePoints() const { return m_remainingMovePoints; }
		TileDistance getFullMovePoints() const { return m_movePoints; }
		Shots getHalfShots() const;
		TileDistance getHalfMovePoints() const;
		Shots getRemainingShots() const { return m_remainingShots; }
		void changeStateByMovement(const TileDistance& distance);
		void changeStateByShooting(const Shots& shots);
	private:
		const TileDistance m_movePoints;
		const Shots m_rateOfFire;
		TileDistance m_remainingMovePoints;
		Shots m_remainingShots;
	};

	class Unit
	{
	public:
		Unit() = default;
		Unit(const ObjectIdentifier unitID, const  TileDistance movePts, const Shots fireRate) : m_id(unitID), m_actionState(movePts, fireRate) {}
		inline ObjectIdentifier getID() const { return m_id; }
		inline GameTile getPosition() const { return m_position; }
		inline TileDistance getHalfMovePoints() { return m_actionState.getHalfMovePoints(); }
		TileDistance getRemainingMovement() { return m_actionState.getRemainingMovePoints(); }
		TileDistance getFullMovement() { return m_actionState.getFullMovePoints(); }
		void resetActionState() { m_actionState.setState(ActionStateStatus::full); }
		inline PlayerIdentifier getOwnerID() const { return m_owner; }
		std::vector<GameTile>& adjustPathByAvailableMovement(std::vector<GameTile>& pathToDest);

		/** Move to destination with adjustment by available movement.
		* returns adjusted path
		*/
		std::vector<core::GameTile> moveTo(std::vector<GameTile>& pathToDest);
		//inline void setActivityState(const ActionStateStatus& state) { m_status = state;  }
		inline void setOwner(const PlayerIdentifier& id) { m_owner = id; }
		inline void setPosition(const GameTile& pos) { m_position = pos; }
		virtual ~Unit() = default;
		HealthPoints m_healthPts{ 0 };
	protected:
		const ObjectIdentifier m_id{0};
		
		GameTile m_position;
		PlayerIdentifier m_owner{0};
		UnitActionState m_actionState;
		// movementPoint = 5;
        // rateOfFire = 3;
		// move move - moveP = 5, shots = 0
		// move fire  - moveP = 3, shots = 2
		// fire fire - moveP = 0, shots = 3 

		// is turret rotateable
		// line of sight, arc of fire 120 deg  by the edge of hex
	};



}

