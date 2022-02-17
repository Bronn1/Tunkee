#pragma once

#include "unit.h"
#include "data_types.h"

#include <map>
#include <optional>
#include <coroutine>
#include <functional>

using UnitPtr = std::unique_ptr<core::Unit>;

namespace core {
	struct IdentifierGenerator {
		struct promise_type {
			using coroHandle = std::coroutine_handle<promise_type>;
			unsigned m_value;
			IdentifierGenerator get_return_object() {
				return { .m_handle = coroHandle::from_promise(*this) };
			}
			std::suspend_never initial_suspend() { return {}; }
			std::suspend_never final_suspend() noexcept { return {}; }
			void unhandled_exception() {}
			void return_void() noexcept {}
			std::suspend_always yield_value(unsigned value)
			{
				m_value = value;
				return {};
			}

		};
		using coroHandle = std::coroutine_handle<promise_type>;
		coroHandle m_handle;
	};

	/** Class holds all units in current game
	* Unit id will be reassigned while unit being added to manager
	* to keep unique ids even in multiplayer game 
	* To generate id uses simple coroutine with co_yeild 
	*/
	class UnitManager 
	{
	public:
		UnitManager();
		std::optional<Unit*> getUnitIfExist(const UnitIdentifier unitId) const;
		std::vector<UnitIdentifier> getActiveUnitsForPlayer(const PlayerIdentifier playerId) const;
		bool hasActiveUnits(const PlayerIdentifier& playerId) const;
		int countActiveUnitsOwnedBy(const PlayerIdentifier& playerId) const;
		void passNextTurnToUnits();
		void setUnitsActions(const ActionStatus state);
		std::vector<UnitIdentifier> getUnitIDs() const;
		std::vector<UnitIdentifier> getUnitIDsForPlayer(const PlayerIdentifier playerId) const;
		UnitIdentifier addUnit(UnitPtr unit);

	private:
		std::map < UnitIdentifier, UnitPtr, Comparator<UnitIdentifier>> m_units{};
		std::function< IdentifierGenerator()> m_generator;
		IdentifierGenerator::coroHandle m_generatorHandle;
	};
}

