#include "unit_manager.h"

std::optional<core::Unit*> core::UnitManager::getUnitIfExist(const ObjectIdentifier& unitId) const
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
