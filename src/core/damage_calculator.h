#pragma once

#include "data_types.h"
#include "unit.h"
#include "damage_handler.h"

#include <unordered_map>
#include <memory>
#include <functional>


namespace core {
	using HandlerName = std::string_view;

	class DamageCalculator 
	{
	public:
		using DamageHandlerPtr = std::unique_ptr<DamageHandler>;
		DamageCalculator();
		HandlerName unitTypeToHandlerName(UnitType unit);
	private:
		std::unordered_map<HandlerName, DamageHandlerPtr> m_damageHandlers;

	};
}

