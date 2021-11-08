//
// Created by ä on 02.11.2021.
//

#include "unit_manager.h"

std::optional<core::Unit*> core::UnitManager::getUnitIfExist(const ObjectIdentifier& unitId) const
{
	if (m_units.contains(unitId))
	{
		return { m_units.at(unitId).get() };
	}
	else
	{
		std::cerr << "Unit with id: " << unitId.identifier << " does not exists\n"; // // TODO notify observer about existing units to sync up (not implemented yet)
		return std::nullopt;
	}
}
