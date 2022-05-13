#include "damage_probability_table.h"
#include "unit_damage_system_strategy.h"
#include "exceptions.h"

void core::DamageProbabilityTable::loadProbabilitiesFromFile(std::string_view filename)
{
}

void core::DamageProbabilityTable::calcEvent(const ThreatLevel& lvl)
{
    std::cout << "calcEvent\n";
}

int core::DamageProbabilityTable::getOverallProbabilitySize(const ThreatLevel& lvl) const
{
    return std::accumulate(begin(m_damageProbabilities.at(lvl)), end(m_damageProbabilities.at(lvl)), 0,
           [](int init, const Probability& probability) {
               return  init + probability.probability; 
           });
}

int core::DamageProbabilityTable::getProbabilityTo(const ThreatLevel& lvl, const UnitDamageName columnName) const
{
    if (!m_damageProbabilities.contains(lvl)) {
        throw core::DamageCalculatorLogicException("Cannot find table of damage probabilities for threat level: " + std::to_string(lvl.level));
    }
    
    auto probIt = std::ranges::find_if(m_damageProbabilities.at(lvl), [&columnName](const auto& probability) { return columnName == probability.name; });
    return (probIt != end(m_damageProbabilities.at(lvl))) ? (*probIt).probability : 0;
}

std::string_view core::DamageProbabilityTable::getDestroyedPart(const ThreatLevel& lvl, int rolledProbability) const
{ 
    int prevNumber = 0;

    auto ifRolledValueBetweenPrevAndCurrent = [&rolledProbability, &prevNumber](const Probability& probability) {
        if (rolledProbability >= prevNumber && rolledProbability < prevNumber + probability.probability)
        {
            prevNumber += probability.probability;
            return true;
        }
        prevNumber += probability.probability;
        return false;
    };

    if (auto resIt = std::ranges::find_if(m_damageProbabilities.at(lvl), ifRolledValueBetweenPrevAndCurrent); resIt != end(m_damageProbabilities.at(lvl)))
        return (*resIt).name;
    else
        return tankDamageSystem::kRicochet;
}

void core::DamageProbabilityTable::fillTankTableWithoutFile()
{
    m_damageProbabilities[ThreatLevel{ 0 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 2}, Probability{tankDamageSystem::kBurning, 3}, Probability{tankDamageSystem::kTrack, 6}, Probability{tankDamageSystem::kDriver, 3},
         Probability{tankDamageSystem::kCommander, 3}, Probability{tankDamageSystem::kLoader, 1}, Probability{tankDamageSystem::kGunner, 3}, Probability{tankDamageSystem::kRadioman, 2},
         Probability{tankDamageSystem::kTransmission, 2}, Probability{tankDamageSystem::kEngine, 1}, Probability{tankDamageSystem::kTurretJammed, 4}, Probability{tankDamageSystem::kGunDestroyed, 4},
         Probability{tankDamageSystem::kScope, 3}, Probability{tankDamageSystem::kCrewKilled, 2}, Probability{tankDamageSystem::kCrewShellShocked, 2}, Probability{tankDamageSystem::kRicochet, 15} };

    m_damageProbabilities[ThreatLevel{ -3 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 0}, Probability{tankDamageSystem::kBurning, 0}, Probability{tankDamageSystem::kTrack, 9}, Probability{tankDamageSystem::kDriver, 1},
         Probability{tankDamageSystem::kCommander, 2}, Probability{tankDamageSystem::kLoader, 0}, Probability{tankDamageSystem::kGunner, 1}, Probability{tankDamageSystem::kRadioman, 1},
         Probability{tankDamageSystem::kTransmission, 0}, Probability{tankDamageSystem::kEngine, 0}, Probability{tankDamageSystem::kTurretJammed, 6}, Probability{tankDamageSystem::kGunDestroyed, 1},
         Probability{tankDamageSystem::kScope, 7}, Probability{tankDamageSystem::kCrewKilled, 0}, Probability{tankDamageSystem::kCrewShellShocked, 0}, Probability{tankDamageSystem::kRicochet, 24} };

    m_damageProbabilities[ThreatLevel{ -2 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 0}, Probability{tankDamageSystem::kBurning, 1}, Probability{tankDamageSystem::kTrack, 8}, Probability{tankDamageSystem::kDriver, 2},
         Probability{tankDamageSystem::kCommander, 2}, Probability{tankDamageSystem::kLoader, 0}, Probability{tankDamageSystem::kGunner, 2}, Probability{tankDamageSystem::kRadioman, 2},
         Probability{tankDamageSystem::kTransmission, 2}, Probability{tankDamageSystem::kEngine, 0}, Probability{tankDamageSystem::kTurretJammed, 5}, Probability{tankDamageSystem::kGunDestroyed, 2},
         Probability{tankDamageSystem::kScope, 6}, Probability{tankDamageSystem::kCrewKilled, 0}, Probability{tankDamageSystem::kCrewShellShocked, 0}, Probability{tankDamageSystem::kRicochet, 22} };

    m_damageProbabilities[ThreatLevel{ -1 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 1}, Probability{tankDamageSystem::kBurning, 2}, Probability{tankDamageSystem::kTrack, 7}, Probability{tankDamageSystem::kDriver, 2},
         Probability{tankDamageSystem::kCommander, 3}, Probability{tankDamageSystem::kLoader, 1}, Probability{tankDamageSystem::kGunner, 2}, Probability{tankDamageSystem::kRadioman, 2},
         Probability{tankDamageSystem::kTransmission, 2}, Probability{tankDamageSystem::kEngine, 0}, Probability{tankDamageSystem::kTurretJammed, 4}, Probability{tankDamageSystem::kGunDestroyed, 3},
         Probability{tankDamageSystem::kScope, 4}, Probability{tankDamageSystem::kCrewKilled, 4}, Probability{tankDamageSystem::kCrewShellShocked, 1}, Probability{tankDamageSystem::kRicochet, 19} };

    m_damageProbabilities[ThreatLevel{ 1 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 3}, Probability{tankDamageSystem::kBurning, 4}, Probability{tankDamageSystem::kTrack, 5}, Probability{tankDamageSystem::kDriver, 3},
         Probability{tankDamageSystem::kCommander, 4}, Probability{tankDamageSystem::kLoader, 2}, Probability{tankDamageSystem::kGunner, 3}, Probability{tankDamageSystem::kRadioman, 2},
         Probability{tankDamageSystem::kTransmission, 3}, Probability{tankDamageSystem::kEngine, 2}, Probability{tankDamageSystem::kTurretJammed, 2}, Probability{tankDamageSystem::kGunDestroyed, 3},
         Probability{tankDamageSystem::kScope, 2}, Probability{tankDamageSystem::kCrewKilled, 3}, Probability{tankDamageSystem::kCrewShellShocked, 3}, Probability{tankDamageSystem::kRicochet, 11} };

    m_damageProbabilities[ThreatLevel{ 2 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 4}, Probability{tankDamageSystem::kBurning, 5}, Probability{tankDamageSystem::kTrack, 4}, Probability{tankDamageSystem::kDriver, 4},
         Probability{tankDamageSystem::kCommander, 4}, Probability{tankDamageSystem::kLoader, 2}, Probability{tankDamageSystem::kGunner, 3}, Probability{tankDamageSystem::kRadioman, 2},
         Probability{tankDamageSystem::kTransmission, 3}, Probability{tankDamageSystem::kEngine, 3}, Probability{tankDamageSystem::kTurretJammed, 1}, Probability{tankDamageSystem::kGunDestroyed, 2},
         Probability{tankDamageSystem::kScope, 2}, Probability{tankDamageSystem::kCrewKilled, 4}, Probability{tankDamageSystem::kCrewShellShocked, 4}, Probability{tankDamageSystem::kRicochet, 8} };


    m_damageProbabilities[ThreatLevel{ 3 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 5}, Probability{tankDamageSystem::kBurning, 6}, Probability{tankDamageSystem::kTrack, 3}, Probability{tankDamageSystem::kDriver, 4},
         Probability{tankDamageSystem::kCommander, 5}, Probability{tankDamageSystem::kLoader, 3}, Probability{tankDamageSystem::kGunner, 3}, Probability{tankDamageSystem::kRadioman, 2},
         Probability{tankDamageSystem::kTransmission, 3}, Probability{tankDamageSystem::kEngine, 3}, Probability{tankDamageSystem::kTurretJammed, 0}, Probability{tankDamageSystem::kGunDestroyed, 1},
         Probability{tankDamageSystem::kScope, 1}, Probability{tankDamageSystem::kCrewKilled, 5}, Probability{tankDamageSystem::kCrewShellShocked, 5}, Probability{tankDamageSystem::kRicochet, 6} };




}
