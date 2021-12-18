#include "unit_manager.h"
#include <ranges>
#include <algorithm>


core::IdentifierGenerator generator()
{
	for (unsigned i = 0;; i++)
	{
		co_yield i;
	}
}

core::UnitManager::UnitManager()
{
	m_generator = generator;
	m_generatorHandle = m_generator().m_handle;
	m_units.insert({ UnitIdentifier{0}, std::make_unique<NullUnit>() });
}

std::optional<core::Unit*> core::UnitManager::getUnitIfExist(const UnitIdentifier& unitId) const
{
	if (m_units.contains(unitId))
	{
		return { m_units.at(unitId).get() };
	}
	else
	{
		std::cerr << "Unit with id: " << unitId.identifier << " does not exists\n";
		return std::nullopt;
	}
}

std::vector<UnitIdentifier> core::UnitManager::getActiveUnitsForPlayer(const PlayerIdentifier& playerId) const
{
	std::vector<UnitIdentifier> activeUnits{};
	for (auto& [id, unitPtr] : m_units)
	{
		if (playerId == unitPtr->getOwnerID() && unitPtr->hasActionLeft())
			activeUnits.push_back(id);

	}
	return activeUnits;
}

bool core::UnitManager::hasActiveUnits(const PlayerIdentifier& playerId) const
{
	auto hasUnitAction = [&playerId](auto& idUnitPair) { return (playerId == idUnitPair.second->getOwnerID() && idUnitPair.second->hasActionLeft()); };
	return std::ranges::find_if(m_units, hasUnitAction) != end(m_units);
}

int core::UnitManager::countUnitsOwnerBy(const PlayerIdentifier& playerId) const
{
	auto isOwnedBy = [&playerId](auto& idUnitPair) { return playerId == idUnitPair.second->getOwnerID(); };
	return  std::ranges::count_if(m_units, isOwnedBy);
}

void core::UnitManager::calculateAliveUnitsOnNextTurn()
{
	for (const auto& [id, unit] : m_units)
	{
		std::cout << "Calculating alive units...\n";
	}
}

void core::UnitManager::setUnitsActions(const ActionStateStatus state)
{
	for (const auto& [id, unit] : m_units)
		unit->setActionState(state);
}

UnitIdentifier core::UnitManager::addUnit(UnitPtr unit)
{
	auto& promise = m_generatorHandle.promise();
	m_generatorHandle();
	unit.get()->setUnitID(UnitIdentifier{ promise.m_value });
	std::cout << "Unit added: " << unit.get()->getID().identifier << "\n";
	m_units.insert({ unit.get()->getID(), std::move(unit) });
	return UnitIdentifier{ promise.m_value };
}
