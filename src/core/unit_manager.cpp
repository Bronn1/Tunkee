#include "unit_manager.h"
#include <ranges>



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

UnitIdentifier core::UnitManager::addUnit(UnitPtr unit)
{
	auto& promise = m_generatorHandle.promise();
	m_generatorHandle();
	unit.get()->setUnitID(UnitIdentifier{ promise.m_value });
	std::cout << "Unit added: " << unit.get()->getID().identifier << "\n";
	m_units.insert({ unit.get()->getID(), std::move(unit) });
	return UnitIdentifier{ promise.m_value };
}
