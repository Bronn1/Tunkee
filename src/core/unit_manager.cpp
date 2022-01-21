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

std::optional<core::Unit*> core::UnitManager::getUnitIfExist(const UnitIdentifier unitId) const
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

std::vector<UnitIdentifier> core::UnitManager::getActiveUnitsForPlayer(const PlayerIdentifier playerId) const
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

int core::UnitManager::countActiveUnitsOwnedBy(const PlayerIdentifier& playerId) const
{
	// TODO  return only alive units, cuz destroyed units should be still represented in manager
	auto isOwnedByAndActive = [&playerId](auto& idUnitPair) { return (playerId == idUnitPair.second->getOwnerID() && idUnitPair.second->hasActionLeft()); };
	return  std::ranges::count_if(m_units, isOwnedByAndActive);
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

std::vector<UnitIdentifier> core::UnitManager::getUnitIDs() const
{
	std::vector<UnitIdentifier> unitIds{};
	std::ranges::transform(m_units, std::back_inserter(unitIds), [](const auto& idUnitPair) { return idUnitPair.first; });

	return unitIds;
}

std::vector<UnitIdentifier> core::UnitManager::getUnitIDsForPlayer(const PlayerIdentifier playerId) const
{
	auto view_filter = m_units |  std::views::filter([&playerId](const auto& idUnitPair) { return playerId == idUnitPair.second->getOwnerID(); }) |
		                          std::views::transform([](const auto& idUnitPair) { return idUnitPair.first; });
	std::vector<UnitIdentifier> unitIds(begin(view_filter), end(view_filter));

	return unitIds;

}

UnitIdentifier core::UnitManager::addUnit(UnitPtr unit)
{
	auto& promise = m_generatorHandle.promise();
	m_generatorHandle();
    unit->setUnitID(UnitIdentifier{ promise.m_value });
	std::cout << "Unit added: " << unit.get()->getID().identifier << "\n";
	m_units.insert({ unit.get()->getID(), std::move(unit) });
	return UnitIdentifier{ promise.m_value };
}
