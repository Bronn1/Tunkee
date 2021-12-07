#include "unit.h"

#include <ranges>

bool core::Unit::isUnitHaveFullActionState() const
{
	if (getRemainingMovement() == getFullMovement() &&
		getRemainingShots() == getFullShots())
		return true;
	return false;

}

// maybe better return by ref too, since we working with parameter which is passed by ref to avoid additional copy on return
std::vector<core::GameTile>& core::Unit::adjustPathByAvailableMovement(std::vector<GameTile>& pathToDest)
{// convert to tileDistance
    unsigned costSoFar = 0;
    auto [move] = getRemainingMovement();

    auto rmv_if = [&costSoFar, &move](const GameTile& t) {costSoFar += t.staticCastTypeToInt(); return costSoFar > move; };
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

void core::UnitActionState::setState(const ActionStateStatus& state)
{
	if (state == ActionStateStatus::full) {
		m_remainingMovePoints = m_fullMovePoints;
		m_remainingShots = m_rateOfFire;
	}
	else if (state == ActionStateStatus::half) {
		m_remainingMovePoints.distance = m_fullMovePoints.distance / 2;
		m_remainingShots.shots = m_rateOfFire.shots / 2;
	}
	else {
		m_remainingMovePoints = { 0 };
		m_remainingShots = { 0 };
	}
}

TileDistance core::UnitActionState::getRemainingMoveInFirstHalf() const
{
	TileDistance remainingInFirstAction{ getRemainingMovePoints() - getHalfMovePoints() };
	return TileDistance((m_fullMovePoints.distance % 2 == 0) ? (remainingInFirstAction.distance) : (remainingInFirstAction.distance + 1));
}

Shots core::UnitActionState::getHalfShots() const
{
	return Shots((m_rateOfFire.shots % 2 == 0) ? (m_rateOfFire.shots / 2) : (m_rateOfFire.shots / 2) + 1);
}

TileDistance core::UnitActionState::getHalfMovePoints() const
{
	return TileDistance((m_fullMovePoints.distance % 2 == 0) ? (m_fullMovePoints.distance / 2) : (m_fullMovePoints.distance / 2) + 1);
}

void core::UnitActionState::changeStateByMovement(const TileDistance& distance)
{
	if (distance > m_remainingMovePoints)
		m_remainingMovePoints = { 0 };
	else
		m_remainingMovePoints -= distance;

	TileDistance halfDistance = getHalfMovePoints();
	if ((m_remainingMovePoints < m_fullMovePoints) && 
		(m_remainingMovePoints > halfDistance))
	{
		m_remainingShots = { 0 };
	}
	else if(m_remainingMovePoints <= halfDistance)
	{
		if (m_remainingShots >= getHalfShots())
		{
			m_remainingShots.shots -= getHalfShots().shots;
		}
		else
		{
			//error?
		 }
		//m_remainingShots = { (m_remainingShots.shots / 2) + 1 };
	}
}
