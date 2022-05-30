#include "unit_manager.h"
#include <ranges>
#include <algorithm>


namespace ranges = std::ranges;
namespace views = std::views;

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
    if (!m_units.contains(unitId)) {
        std::cerr << "Unit with id: " << unitId.identifier << " does not exists\n";
        return std::nullopt;
    }

    return { m_units.at(unitId).get() };
}

bool core::UnitManager::hasActiveUnits(const PlayerIdentifier& playerId) const
{
    auto hasUnitActionLeft = [&playerId](auto& idUnitPair) { return (playerId == idUnitPair.second->getOwnerID() && idUnitPair.second->hasActionLeft()); };
    return ranges::find_if(m_units, hasUnitActionLeft) != end(m_units);
}

int core::UnitManager::countActiveUnitsOwnedBy(const PlayerIdentifier& playerId) const
{
    auto isOwnedByAndActive = [&playerId](const auto& idUnitPair) { return (playerId == idUnitPair.second->getOwnerID() && idUnitPair.second->hasActionLeft()); };
    return  ranges::count_if(m_units, isOwnedByAndActive);
}

void core::UnitManager::passNextTurnToUnits()
{
    for (const auto& [id, unit] : m_units) {
        unit->nextTurn();
    }
}

void core::UnitManager::setUnitsActions(const ActionStatus state)
{
    for (const auto& [id, unit] : m_units)
        unit->setActionState(state);
}

int core::UnitManager::countAliveUnits(const PlayerIdentifier& playerId, PointOfView pov) const
{
    return ranges::count_if(m_units, [&playerId, &pov]( const auto& idUnitPair) {return playerId == idUnitPair.second->getOwnerID() && idUnitPair.second->isAlive(pov); });
}

std::vector<UnitIdentifier> core::UnitManager::getUnitIDs() const
{
    std::vector<UnitIdentifier> unitIds{};
    std::ranges::transform(m_units, std::back_inserter(unitIds), [](const auto& idUnitPair) { return idUnitPair.first; });

    return unitIds;
}

std::vector<UnitIdentifier> core::UnitManager::getUnitIDsForPlayer(const PlayerIdentifier playerId) const
{
    auto view_filter = m_units | views::filter([&playerId](const auto& idUnitPair) { return playerId == idUnitPair.second->getOwnerID(); }) |
                       views::transform([](const auto& idUnitPair) { return idUnitPair.first; });
    std::vector<UnitIdentifier> unitIds(begin(view_filter), end(view_filter));

    return unitIds;

}

std::vector<UnitIdentifier> core::UnitManager::getActiveUnitsForPlayer(const PlayerIdentifier playerId) const
{
    std::vector<UnitIdentifier> activeUnits{};
    for (auto& [id, unitPtr] : m_units) {
        if (playerId == unitPtr->getOwnerID() && unitPtr->hasActionLeft())
            activeUnits.push_back(id);
    }
    return activeUnits;
}

void core::UnitManager::setAllDamageVisible()
{
    for (auto& [id, unit] : m_units) 
        unit->setDamageVisibleForEnemy();
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
std::vector<UnitIdentifier> core::UnitManager::getAllUnits() const
{
    std::vector<UnitIdentifier> res {};
    ranges::transform(m_units, std::back_inserter(res), 
        [](const auto& unitPair) { 
            return unitPair.first; 
        });
    return res;
}
// показываю в момент попадания и накопленные
// +3 on red
// ctolbci ikonka text
// details dlya ver popadani9 po chast9m