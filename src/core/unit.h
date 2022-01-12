#pragma once

#include "data_types.h"
#include "events.h"
#include "game_tile.h"
#include "unit_state.h"

namespace core 
{
	/** @brief Contains logic of possible action unit can do in
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
		using UnitStatePtr = std::unique_ptr< UnitState>;
		enum class ActionState{
			NoActionsPerformed,
			MovePerformed,
			ShotPerformed,
			BothActionsPerformed,
		};
		using enum ActionState;
		UnitActionState(const  TileDistance movePts, const Shots fireRate)
			: m_fullMovePoints(movePts), m_rateOfFire(fireRate), m_remainingMovePoints(movePts), m_remainingShots(fireRate) {}
		void setState(const ActionStateStatus& state);
		//TileDistance getRemainingMovePoints() const { return m_remainingMovePoints; }
		TileDistance getRemainingMoveInFirstHalf() const;
		//TileDistance getFullMovePoints() const { return m_fullMovePoints; }
		TileDistance getHalfMovePointsRoundedUp() const;
		TileDistance getHalfMovePoints() const { return TileDistance{ m_state->getMoveDistanceWithFine(m_fullMovePoints).distance / 2 }; }

		Shots getHalfShotsRoundedUp() const;
		Shots getHalfShots() const { return Shots{ m_state->getRateOfFireWithFine(m_rateOfFire).shots / 2 };} 
		//Shots getRemainingShots() const { return m_remainingShots; }
		//Shots getRateOfFire() const { return m_rateOfFire; }
		void changeStateByMovement(const TileDistance& distance);
		void changeStateByShooting(const Shots& shots);
		void setStateSystem(UnitStatePtr state) { m_state = std::move(state); }

		friend class Unit;
	private:
		const TileDistance m_fullMovePoints;
		const Shots m_rateOfFire;
		TileDistance m_remainingMovePoints;
		Shots m_remainingShots;
		ActionState m_actionState{ NoActionsPerformed };
		UnitStatePtr m_state;
	};

	/*class TankLifeSupportModel
	{
	public:
		friend class Unit;
	private:
		std::vector<UnitPart> m_possibleDamage;
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
		inline Angle getBodyRotation() const { return m_bodyRotation; }
		inline Angle getGunRotation() const { return m_gunRotation; }
		inline void setUnitRotation(const Angle& angle) { m_bodyRotation = angle; m_gunRotation = angle ; std::cout << m_bodyRotation.angle << " " << m_gunRotation.angle << " model\n";};
		inline void setGunRotation(const Angle& angle) { m_gunRotation = angle; std::cout << m_bodyRotation.angle << " " << m_gunRotation.angle << " model2\n"; }

		inline TileDistance getHalfMovePointsRoundedUp() const  { return m_actionState.getHalfMovePointsRoundedUp(); }
		TileDistance getRemainingMovement() const  { return m_actionState.m_remainingMovePoints; }
		TileDistance getRemainingMovementInFirstHalf() const { return m_actionState.getRemainingMoveInFirstHalf(); }
		TileDistance getFullMovement() const { return m_actionState.m_state->getMoveDistanceWithFine(m_actionState.m_fullMovePoints); }
		inline bool canMove() const { return (getRemainingMovement() > TileDistance{ 0 } && m_actionState.m_state->canMove()) ? true : false; }
		inline bool canShoot() const { return ( getRemainingShots() > Shots{ 0 } && m_actionState.m_state->canShot()) ? true : false; }
		bool hasActionLeft() const { return canMove() || canShoot(); }

		Shots getRateOfFire() const { return m_actionState.m_state->getRateOfFireWithFine(m_actionState.m_rateOfFire); }
		Shots getRemainingShots() const { return m_actionState.m_remainingShots; }
		Shots getHalfShotsRoundedUp() const { return m_actionState.getHalfShotsRoundedUp(); }
		bool isUnitHaveFullActionState() const;
		void setActionState(const ActionStateStatus state) { m_actionState.setState(state); }
		inline PlayerIdentifier getOwnerID() const { return m_owner; }
		bool isAlive() const;
		int getArmor(const Angle& attackingAngle) const;
		Attack getAttack() const { return m_attack; }
		UnitPart::State getScopeState() const {  return m_actionState.m_state->getScopeState(); }

		void applyDamage(const std::string_view damageType);
		/** Move to destination with adjustment by available movement.
		* returns adjusted path
		*/
		virtual std::vector<core::GameTile> moveTo(std::vector<GameTile>& pathToDest);
		bool shots(const Shots& shots);
		//inline void setActivityState(const ActionStateStatus& state) { m_status = state;  }
		inline void setOwner(const PlayerIdentifier id) { m_owner = id; }
		inline void setPosition(const GameTile& pos) { m_position = pos; }
		inline void setUnitID(const UnitIdentifier id) { m_id = id; }
		inline void setArmor(const Armor armor) { m_armor = armor; }
		inline void setAttack(const Attack attack) { m_attack = attack; }
		virtual ~Unit() = default;


		TileDistance getRangeOfFire() const { return m_rangeOfFire; }
		void  setRangeOfFire(const TileDistance shootingDistance) { m_rangeOfFire = shootingDistance; }
	protected:
		std::vector<GameTile>& adjustPathByAvailableMovement(std::vector<GameTile>& pathToDest);

		UnitIdentifier m_id;
		GameTile m_position;
		PlayerIdentifier m_owner{0};
		UnitActionState m_actionState;
		UnitType m_type;
		// TODO add restrictions to rotation in case of possible damage to turret
		Angle m_bodyRotation{ 0 };
		Angle m_gunRotation{ 0 };
		TileDistance m_rangeOfFire{ 20 };
		Armor m_armor{ 4, 2 };
		Attack m_attack{ 3 };


		// is turret rotateable
		// line of sight, arc of fire 120 deg  by the edge of hex
	};

	class NullUnit : public core::Unit {
	public:
		NullUnit() : Unit(UnitIdentifier{ 0 }, TileDistance{ 0 }, Shots{ 1 }) {
			setOwner(PlayerIdentifier{ 0 });
			m_actionState.setState(ActionStateStatus::empty);
			m_actionState.setStateSystem(std::make_unique< tank_state_system::TankState>(Crew{ 5, 3 }));
		}
		bool hasActionLeft() const  { return  false; }


	};

	using DamageTypename = std::string_view;

	class TankUnit : public core::Unit {
	public:
		TankUnit(UnitIdentifier id, TileDistance movePoints, Shots rateOfFire);

	};



}

