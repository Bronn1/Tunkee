#pragma once

#include "data_types.h"
#include "events.h"
#include "game_tile.h"
#include "unit_damage_type.h"

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
		//TileDistance getRemainingMovePoints() const { return m_remainingMovePoints; }
		TileDistance getRemainingMoveInFirstHalf() const;
		//TileDistance getFullMovePoints() const { return m_fullMovePoints; }
		TileDistance getHalfMovePoints() const;

		Shots getHalfShots() const;
		//Shots getRemainingShots() const { return m_remainingShots; }
		//Shots getRateOfFire() const { return m_rateOfFire; }
		void changeStateByMovement(const TileDistance& distance);
		void changeStateByShooting(const Shots& shots);

		friend class Unit;
	private:
		const TileDistance m_fullMovePoints;
		const Shots m_rateOfFire;
		TileDistance m_remainingMovePoints;
		Shots m_remainingShots;
	};

	/*class TankLifeSupportModel
	{
	public:
		friend class Unit;
	private:
		std::vector<UnitDamageType> m_possibleDamage;
	};
	*/
	class UnitLogger
	{

	};

	enum class UnitType: unsigned char
	{
		Tank
	};



	class Unit
	{
	public:
		Unit() = default;
		Unit(const UnitIdentifier unitID, const  TileDistance movePts, const Shots fireRate) : m_id(unitID), m_actionState(movePts, fireRate) {}
		inline UnitIdentifier getID() const { return m_id; }
		UnitType getType() const { return m_type; }
		inline GameTile getPosition() const { return m_position; }
		inline TileDistance getHalfMovePoints() const  { return m_actionState.getHalfMovePoints(); }
		TileDistance getRemainingMovement() const  { return m_actionState.m_remainingMovePoints; }
		TileDistance getRemainingMovementInFirstHalf() const { return m_actionState.getRemainingMoveInFirstHalf(); }
		TileDistance getFullMovement() const { return m_actionState.m_fullMovePoints; }
		inline bool canMove() const { return (getRemainingMovement() > TileDistance{ 0 }) ? true : false; }
		bool hasActionLeft() const { return canMove() || (getRemainingShots() > Shots{ 0 }) ? true : false; }

		Shots getRateOfFire() const { return m_actionState.m_rateOfFire; }
		Shots getRemainingShots() const { return m_actionState.m_remainingShots; }
		Shots getHalfShots() const { return m_actionState.getHalfShots(); }
		bool isUnitHaveFullActionState() const;
		void setActionState(const ActionStateStatus state) { m_actionState.setState(state); }
		inline PlayerIdentifier getOwnerID() const { return m_owner; }

		/** Move to destination with adjustment by available movement.
		* returns adjusted path
		*/
		virtual std::vector<core::GameTile> moveTo(std::vector<GameTile>& pathToDest);
		//inline void setActivityState(const ActionStateStatus& state) { m_status = state;  }
		inline void setOwner(const PlayerIdentifier id) { m_owner = id; }
		inline void setPosition(const GameTile& pos) { m_position = pos; }
		inline void setUnitID(const UnitIdentifier id) { m_id = id; }
		virtual ~Unit() = default;

	protected:
		std::vector<GameTile>& adjustPathByAvailableMovement(std::vector<GameTile>& pathToDest);

		UnitIdentifier m_id;
		GameTile m_position;
		PlayerIdentifier m_owner{0};
		UnitActionState m_actionState;
		UnitType m_type;
		// TODO or maybe make sense to add damage here only when unit actually take it 
		// or maybe switch to unordered map
		// 
		std::vector<UnitDamageType> m_possibleDamage;

		// is turret rotateable
		// line of sight, arc of fire 120 deg  by the edge of hex
	};

	class NullUnit : public core::Unit {
	public:
		NullUnit() : Unit(UnitIdentifier{ 0 }, TileDistance{ 0 }, Shots{ 1 }) {
			setOwner(PlayerIdentifier{ 0 });
			m_actionState.setState(ActionStateStatus::empty);
		}
		bool hasActionLeft() const  { return  false; }

	};

	class TankUnit : public core::Unit {
	public:
		TankUnit(UnitIdentifier id, TileDistance dis);

	};



}

