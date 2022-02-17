#pragma once

#include "data_types.h"
#include "events.h"
#include "game_tile.h"
#include "unit_damage_system_strategy.h"

namespace core 
{

	class GameBoard;
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
		using DamageSystemStrategyPtr = std::unique_ptr< IDamageSystemStrategy>;
		enum class ActionState{
			NoActionsPerformed,
			MovePerformed,
			ShotPerformed,
			BothActionsPerformed,
		};
		using enum ActionState;
		UnitActionState(const  TileDistance movePts, const Shots fireRate)
			: m_fullMovePoints(movePts), m_rateOfFire(fireRate), m_remainingMovePoints(movePts), m_remainingShots(fireRate) {}

		virtual void changeStateByMovement(const TileDistance& distance) = 0;
		virtual void changeStateByShooting(const Shots& shots) = 0;

	public: // Mandatory implementations
		TileDistance getRemainingMoveInFirstHalf() const;
		TileDistance getHalfMovePointsRoundedUp() const;
		TileDistance getHalfMovePoints() const { return TileDistance{ m_damageSystemStrategy->getMoveDistanceWithFine(m_fullMovePoints).distance / 2 }; }
		Shots getHalfShotsRoundedUp() const;
		Shots getHalfShots() const { return Shots{ m_damageSystemStrategy->getRateOfFireWithFine(m_rateOfFire).shots / 2 }; }
		void setDamageStrategy(DamageSystemStrategies strategy, Crew crewAmount);
		void setActionState(const ActionStatus& state);
		virtual ~UnitActionState() = default;
		friend class Unit;
	protected:
		void defaultChangeStateByMovement(const TileDistance& distance);
		void defaultChangeStateByShooting(const Shots& shots);
	private:
		const TileDistance m_fullMovePoints;
		const Shots m_rateOfFire;
		TileDistance m_remainingMovePoints;
		Shots m_remainingShots;
		ActionState m_actionState{ NoActionsPerformed };
	protected:
		DamageSystemStrategyPtr m_damageSystemStrategy;
	};

	struct UnitTurnInfo{
		GameTile prevPosition;
		GameTile curPosition;
		Shots shotsPerformed{ 0 };
		std::vector<std::string_view> damageDone{};
	};

	class UnitLogger
	{
	public:
		// pop
		// isCurrentTurnEmpty()
		// 
		//std::string getFullInformation() const;
	private:
		std::deque< UnitTurnInfo> m_turnsHistory{};
		// get unit infoMessage
		// undo checkIfEnemysecretsFind for fog of war check if we can undo last move or shot or rotation should be function in game engine undo command(pop stack( with flag undoable or not, 
		// if yes pass to logger and unit to change state
		//getlastmove
		// getlastShot
		// 
	};

	enum class UnitType: unsigned char
	{
		Tank
	};


	class Unit : public UnitActionState
	{
	public:
		Unit(const UnitIdentifier unitID, const  TileDistance movePts, const Shots fireRate) : m_id(unitID), UnitActionState(movePts, fireRate) {}

		virtual bool isTargetInLineOfSight(const GameBoard& board, const GameTile& target) const = 0;
		virtual int  getArmor(const Angle& attackingAngle) const = 0;
		virtual void setGunRotation(const Angle& angle) = 0;
		virtual void applyDamage(const std::string_view damageType) = 0;
		/** Move to destination with adjustment by available movement.
		* returns adjusted path
		*/
		virtual std::vector<core::GameTile> moveTo(const MoveToAction* moveToCmd, GameBoard& board) = 0;
		virtual void rotateTo(const GameTile& destination, GameBoard& board) = 0;
		virtual bool shoot(const Shots& shots) = 0;
		virtual void nextTurn() = 0;
		virtual void setUnitRotation(const HexVertexNumber rotation) = 0;
		virtual MoveAreaInfo getMoveArea(const GameBoard& board) const = 0;

		inline bool canMove() const; // maybe add to interface these too
		inline bool canShoot() const;

	protected:
		void defaultApplyDamage(const std::string_view damageType);
		std::vector<core::GameTile> defaultMoveTo(const MoveToAction* moveToCmd, GameBoard& board);
		void defaultRotateTo(const GameTile& destination, GameBoard& board);
		bool defaultShoot(const Shots& shots);
		void defaultNextTurn();
		void defaultSetUnitRotation(const HexVertexNumber rotation);
		int  defaultGetArmor(const Angle& attackingAngle) const;
		MoveAreaInfo defaultGetMoveArea(const GameBoard& board) const;
	public:
		// Mandatory implementation
		bool isAlive(PointOfView pointOfView) const { return m_damageSystemStrategy->isAlive(pointOfView); }
		ActionStatus isUnitHaveFullActionState() const;
		Attack getAttack() const { return m_attack; }
		inline UnitIdentifier getID() const { return m_id; }
		UnitType getType() const { return m_type; }
		inline GameTile getPosition() const { return m_position; }
		HexVertexNumber getUnitVertexRotation() const { return m_unitRotation; }
		TileDistance getRangeOfFire() const { return m_rangeOfFire; }
		Shots getRateOfFire() const { return m_damageSystemStrategy->getRateOfFireWithFine(m_rateOfFire); }
		inline Angle getGunRotation() const { return m_gunRotation; }
		inline PlayerIdentifier getOwnerID() const { return m_owner; }
		TileDistance getFullMovement() const { return m_damageSystemStrategy->getMoveDistanceWithFine(m_fullMovePoints); }
		Shots getRemainingShots() const { return m_remainingShots; }
		TileDistance getRemainingMovement() const { return m_remainingMovePoints; }
		void setDamageState(const DamageType damageType, const  UnitPart::State  state) { m_damageSystemStrategy->setDamageState(damageType, state); }
		inline void setOwner(const PlayerIdentifier id) { m_owner = id; }
		inline void setPosition(const GameTile& pos) { m_position = pos; }
		inline void setUnitID(const UnitIdentifier id) { m_id = id; }
		inline void setArmor(const Armor armor) { m_armor = armor; }
		inline void setAttack(const Attack attack) { m_attack = attack; }
		void setBodyRotation(const HexVertexNumber rotation) { m_unitRotation = rotation; }
		void  setRangeOfFire(const TileDistance shootingDistance) { m_rangeOfFire = shootingDistance; }
		bool hasActionLeft() const { return canMove() || canShoot(); }
		virtual ~Unit() = default;
	private:
		std::vector<GameTile>& adjustPathByAvailableMovement(std::vector<GameTile>& pathToDest);
		void rotateToVertex(const HexVertexNumber vertex);

		UnitIdentifier m_id;
		GameTile m_position;
		PlayerIdentifier m_owner{0};
		// TODO add restrictions to rotation in case of possible damage to turret
		HexVertexNumber m_unitRotation{ 1 };
		TileDistance m_rangeOfFire{ 20 };
		Armor m_armor{ 4, 2 };
		Attack m_attack{ 3 };
		int  m_rotationCounter{ 0 };
	protected:
		UnitType m_type;
		Angle m_gunRotation{ 0 };
	};


	using DamageTypename = std::string_view;

	class TankUnit : public core::Unit {
	public:
		TankUnit(UnitIdentifier id, TileDistance movePoints, Shots rateOfFire);

		void setGunRotation(const Angle& angle) override;

		void changeStateByMovement(const TileDistance& distance)  override { defaultChangeStateByMovement(distance); }
		void changeStateByShooting(const Shots& shots) override { defaultChangeStateByShooting(shots); }
		void applyDamage(const std::string_view damageType) override { defaultApplyDamage(damageType); }
		std::vector<core::GameTile> moveTo(const MoveToAction* moveToCmd, GameBoard& board) override { return defaultMoveTo(moveToCmd, board); }
		void rotateTo(const GameTile& destination, GameBoard& board) override { defaultRotateTo(destination, board); }
		bool shoot(const Shots& shots) override { return defaultShoot(shots); }
		void nextTurn() override { defaultNextTurn(); }
		void setUnitRotation(const HexVertexNumber rotation) override { defaultSetUnitRotation(rotation); }
		bool isTargetInLineOfSight(const GameBoard& board, const GameTile& target) const override;
		int  getArmor(const Angle& attackingAngle) const override { return defaultGetArmor(attackingAngle); }
		MoveAreaInfo getMoveArea(const GameBoard& board) const  override;
	};


	class NullUnit : public core::Unit {
	public:
		NullUnit() : Unit(UnitIdentifier{ 0 }, TileDistance{ 0 }, Shots{ 1 }) {
			setOwner(PlayerIdentifier{ 0 });
			setActionState(ActionStatus::empty);
			setDamageStrategy(DamageSystemStrategies::TankDamageSystem, Crew{ 0, 0 });
		}

		bool hasActionLeft() const { return  false; }
		void setGunRotation(const Angle& angle) override { }
		void changeStateByMovement(const TileDistance& distance)  override {  }
		void changeStateByShooting(const Shots& shots) override {  }
		void applyDamage(const std::string_view damageType) override { }
		std::vector<core::GameTile> moveTo(const MoveToAction* moveToCmd, GameBoard& board) override { return defaultMoveTo(moveToCmd, board); }
		void rotateTo(const GameTile& destination, GameBoard& board) override { }
		bool shoot(const Shots& shots) override { return false; }
		void nextTurn() override {  }
		void setUnitRotation(const HexVertexNumber rotation) override {  }
		bool isTargetInLineOfSight(const GameBoard& board, const GameTile& target) const override { return false; }
		int  getArmor(const Angle& attackingAngle) const override { return 0; }
		MoveAreaInfo getMoveArea(const GameBoard& board) const  override { return MoveAreaInfo{ }; }
	};


}

