#pragma once

#include "data_types.h"
#include "unit_manager.h"

#include <memory>

using UnitManagerPtr = std::shared_ptr<core::UnitManager>;

namespace core {
	class DamageCalculator : public events::Events
	{
	public:
		DamageCalculator(const UnitManagerPtr unitMngr);
	private:
		UnitManagerPtr m_unitManager;

	};
}

