
#include "damage_calculator.h"

// maybe better to achive the same with meta programming or smth with types..
constexpr std::string_view  kTankDamageHandlerName = "TankDamageHandler";

core::DamageCalculator::DamageCalculator()
{
	m_damageHandlers.insert({ kTankDamageHandlerName , std::make_unique<TankDamageHandler>() });
}

core::HandlerName core::DamageCalculator::unitTypeToHandlerName(UnitType unit)
{
	using enum UnitType;
	switch (unit)
	{
	case Tank: return kTankDamageHandlerName;
	default: return "UndefinedHandler";
	}
}
