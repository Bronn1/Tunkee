#include "unit.h"
#include "pathfinding.h"

#include <ranges>

namespace views = std::views;

bool core::Unit::isUnitHaveFullActionState() const
{
	if (getRemainingMovement() == getFullMovement() &&
		getRemainingShots() == getRateOfFire())
		return true;
	return false;

}

bool core::Unit::isAlive() const
{

	return false;
}

int core::Unit::getArmor(const Angle& attackingAngle) const
{
	// TODO not tested yet
	const Angle kFrontalArmorAngleFrom = Angle{ 210 };
	const Angle kFrontalArmorAngleTo = Angle{ 330 };
	Angle resultAngle = m_bodyRotation - attackingAngle;
	if (resultAngle >= kFrontalArmorAngleFrom && resultAngle <= kFrontalArmorAngleTo)
		return m_armor.m_frontal;
	else
		return m_armor.m_side;
}

void core::Unit::applyDamage(const std::string_view damageType)
{
	bool isFullActionState = isUnitHaveFullActionState();
	m_actionState.m_state->applyDamage(damageType); m_actionState.m_state->canMove();
	//  we arent allow players to have half active units, its either full, or empty
	// so if unit wasnt active yet in this turn we just reset his state after taken damage
	if(isFullActionState) 
		m_actionState.setActionState(ActionStateStatus::full);
}

std::vector<core::GameTile>& core::Unit::adjustPathByAvailableMovement(std::vector<GameTile>& pathToDest)
{// convert to tileDistance
    unsigned costSoFar = 0;
    auto [move] = getRemainingMovement();

    auto rmv_if = [&costSoFar, &move](const GameTile& t) {costSoFar += t.travelCost(); return costSoFar > move; };
    std::erase_if(pathToDest, rmv_if);

    return pathToDest;
}

std::vector<core::GameTile> core::Unit::moveTo(std::vector<GameTile>& pathToDest)
{
	std::vector<GameTile> resultPath{};
	auto adjustedPathVec = adjustPathByAvailableMovement(pathToDest);
	unsigned pathSize = std::size(adjustedPathVec);
	if (pathSize > 0)
	{
		m_actionState.changeStateByMovement(TileDistance{ pathSize });
		resultPath = std::move(adjustedPathVec);
		m_position = resultPath.back();
		return resultPath;
	}
	return resultPath;
}

bool core::Unit::shots(const Shots& shots)
{
	if (m_actionState.m_remainingShots <= Shots{ 0 })
		return false;

	m_actionState.changeStateByShooting(shots);
	return true;
}

void core::Unit::rotateToVertex(const HexVertexNumber vertex)
{
	auto [move] = getRemainingMovement();
	if(move)
	{ 
		m_actionState.changeStateByMovement(TileDistance{ 1 });
		m_unitRotation = vertex;
	}
}

void core::UnitActionState::setActionState(const ActionStateStatus& state)
{
	if (state == ActionStateStatus::full) {
		m_remainingMovePoints = m_state->getMoveDistanceWithFine(m_fullMovePoints);
		m_remainingShots = m_state->getRateOfFireWithFine(m_rateOfFire);
		m_actionState = NoActionsPerformed;
	}
	else {
		m_remainingMovePoints = { 0 };
		m_remainingShots = { 0 };
		m_actionState = BothActionsPerformed;
	}
}

TileDistance core::UnitActionState::getRemainingMoveInFirstHalf() const
{
	TileDistance remainingInFirstAction{ m_remainingMovePoints - getHalfMovePointsRoundedUp() };
	if (remainingInFirstAction.distance == 0) return  remainingInFirstAction;

	return TileDistance((m_state->getMoveDistanceWithFine(m_fullMovePoints).distance % 2 == 0) ? (remainingInFirstAction.distance) : (remainingInFirstAction.distance + 1));
}

Shots core::UnitActionState::getHalfShotsRoundedUp() const
{
	Shots rateOfFireWithFine = m_state->getRateOfFireWithFine(m_rateOfFire);
	return Shots((rateOfFireWithFine.shots % 2 == 0) ? (rateOfFireWithFine.shots / 2) : (rateOfFireWithFine.shots / 2) + 1);
}

TileDistance core::UnitActionState::getHalfMovePointsRoundedUp() const
{
	TileDistance fullMovePtsWithFine = m_state->getMoveDistanceWithFine(m_fullMovePoints);
	return TileDistance((fullMovePtsWithFine.distance % 2 == 0) ? (fullMovePtsWithFine.distance / 2) : (fullMovePtsWithFine.distance / 2) + 1);
}

void core::UnitActionState::changeStateByMovement(const TileDistance& distance)
{
	if (distance > m_remainingMovePoints)
		m_remainingMovePoints = { 0 };
	else
		m_remainingMovePoints -= distance;

	TileDistance halfDistance = getHalfMovePoints();
	if (m_actionState == NoActionsPerformed || m_actionState == MovePerformed) {
		if (m_remainingMovePoints < halfDistance)
			m_remainingShots = { 0 };
		else if (m_remainingShots == m_state->getRateOfFireWithFine(m_rateOfFire))
			m_remainingShots -= getHalfShots();
		m_actionState = MovePerformed;
	}
	else if(m_actionState == ShotPerformed)
	{
		// TODO throw exception if less than zero
		m_remainingShots = getHalfShotsRoundedUp() - (m_state->getRateOfFireWithFine(m_rateOfFire) - m_remainingShots);
		m_actionState = BothActionsPerformed;
	}
}

void core::UnitActionState::changeStateByShooting(const Shots& shots)
{
	if (shots > m_remainingShots)
		m_remainingShots = Shots{ 0 };
	else
		m_remainingShots -= shots;

	Shots halfShots = getHalfShots();
	if (m_actionState == NoActionsPerformed || m_actionState == ShotPerformed) {
		if (m_remainingShots < halfShots)
			m_remainingMovePoints = { 0 };
		else if (m_remainingMovePoints == m_state->getMoveDistanceWithFine(m_fullMovePoints))
			m_remainingMovePoints -= getHalfMovePoints();
		m_actionState = ShotPerformed;
	}
	else if (m_actionState == MovePerformed)
	{
		// TODO throw exception if less than zero
		m_remainingMovePoints = getHalfMovePointsRoundedUp() - (m_state->getMoveDistanceWithFine(m_fullMovePoints) - m_remainingMovePoints);
		m_actionState = BothActionsPerformed;
	}
}

core::TankUnit::TankUnit(UnitIdentifier id, TileDistance dis, Shots rateOfFire)
	: Unit(id, dis, rateOfFire)
{
	m_type = UnitType::Tank;

	m_actionState.setStateSystem(std::make_unique<tank_state_system::TankState>(Crew{ 5, 3 }));
	//applyDamage("Burning");

	//for (const auto& type : kTankDamageTypes)
	//	m_possibleDamage.push_back(UnitPart{type});

	//m_fatalDamage.push_back(UnitPart{ kTankDamageTypes[1] });
}
