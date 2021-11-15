#pragma once

#include "unit.h"
#include "data_types.h"

#include <map>
#include <optional>

using UnitPtr = std::shared_ptr<core::Unit>;

namespace core {
	
	class UnitManager 
	{
	public:
		//UnitManager() = default;
		std::optional<Unit*> getUnitIfExist(const ObjectIdentifier& unitId) const;
		inline void addUnit(UnitPtr unit) { m_units.insert({ unit.get()->getID(), unit }); }

	private:
		std::map < ObjectIdentifier, UnitPtr, Comparator<ObjectIdentifier>> m_units{};
	};
}

