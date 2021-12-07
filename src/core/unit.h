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
	* Example
	* movementPoint = 5;
    * rateOfFire = 3;
	* move move - moveP = 5, shots = 0
	* move fire  - moveP = 3, shots = 2
	* fire fire - moveP = 0, shots = 3 
	*/
	class UnitActionState
	{
	public:
		UnitActionState(const  TileDistance movePts, const Shots fireRate)
			: m_fullMovePoints(movePts), m_rateOfFire(fireRate), m_remainingMovePoints(movePts), m_remainingShots(fireRate) {}
		void setState(const ActionStateStatus& state);
		TileDistance getRemainingMovePoints() const { return m_remainingMovePoints; }
		TileDistance getRemainingMoveInFirstHalf() const;
		TileDistance getFullMovePoints() const { return m_fullMovePoints; }
		TileDistance getHalfMovePoints() const;

		Shots getHalfShots() const;
		Shots getRemainingShots() const { return m_remainingShots; }
		Shots getFullShots() const { return m_rateOfFire; }
		void changeStateByMovement(const TileDistance& distance);
		void changeStateByShooting(const Shots& shots);
	private:
		const TileDistance m_fullMovePoints;
		const Shots m_rateOfFire;
		TileDistance m_remainingMovePoints;
		Shots m_remainingShots;
	};

	class Unit
	{
	public:
		Unit() = default;
		Unit(const UnitIdentifier unitID, const  TileDistance movePts, const Shots fireRate) : m_id(unitID), m_actionState(movePts, fireRate) {}
		inline UnitIdentifier getID() const { return m_id; }
		inline GameTile getPosition() const { return m_position; }
		inline TileDistance getHalfMovePoints() const  { return m_actionState.getHalfMovePoints(); }
		TileDistance getRemainingMovement() const  { return m_actionState.getRemainingMovePoints(); }
		TileDistance getRemainingMovementInFirstHalf() const { return m_actionState.getRemainingMoveInFirstHalf(); }
		TileDistance getFullMovement() const { return m_actionState.getFullMovePoints(); }
		inline bool canMove() const { return (getRemainingMovement() > TileDistance{ 0 }) ? true : false; }

		Shots getFullShots() const { return m_actionState.getFullShots(); }
		Shots getRemainingShots() const { return m_actionState.getRemainingShots(); }
		Shots getHalfShots() const { return m_actionState.getHalfShots(); }
		bool isUnitHaveFullActionState() const;
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
		inline void setUnitID(const UnitIdentifier& id) { m_id = id; }
		virtual ~Unit() = default;
		HealthPoints m_healthPts{ 0 };
	protected:
		UnitIdentifier m_id;
		
		GameTile m_position;
		PlayerIdentifier m_owner{0};
		UnitActionState m_actionState;

		// is turret rotateable
		// line of sight, arc of fire 120 deg  by the edge of hex
	};

	class BasicTank : public core::Unit {
	public:
		BasicTank(UnitIdentifier id, TileDistance dis) : Unit(id, TileDistance{ dis }, Shots{ 3 }) {}

	};



}

