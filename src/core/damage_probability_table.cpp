#include "damage_probability_table.h"

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
        return (*resIt).type;
    else
        return tankDamageSystem::kRicochet;
}

void core::DamageProbabilityTable::fillTankTableWithoutFile()
{
    m_damageProbabilities[ThreatLevel{ 0 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 2}, Probability{tankDamageSystem::kBurning, 3}, Probability{tankDamageSystem::kTrackDamaged, 6}, Probability{tankDamageSystem::kDriverKilled, 3},
         Probability{tankDamageSystem::kCommanderKilled, 3}, Probability{tankDamageSystem::kLoaderKilled, 1}, Probability{tankDamageSystem::kGunnerKilled, 3}, Probability{tankDamageSystem::kRadiomanKilled, 2},
         Probability{tankDamageSystem::kTransmissionDestroyed, 2}, Probability{tankDamageSystem::kEngineDestroyed, 1}, Probability{tankDamageSystem::kTurretJammed, 4}, Probability{tankDamageSystem::kGunDestroyed, 4},
         Probability{tankDamageSystem::kScopeDamaged, 3}, Probability{tankDamageSystem::kCrewKilled, 2}, Probability{tankDamageSystem::kCrewShellShocked, 2}, Probability{tankDamageSystem::kRicochet, 15} };

    m_damageProbabilities[ThreatLevel{ -3 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 0}, Probability{tankDamageSystem::kBurning, 0}, Probability{tankDamageSystem::kTrackDamaged, 9}, Probability{tankDamageSystem::kDriverKilled, 1},
         Probability{tankDamageSystem::kCommanderKilled, 2}, Probability{tankDamageSystem::kLoaderKilled, 0}, Probability{tankDamageSystem::kGunnerKilled, 1}, Probability{tankDamageSystem::kRadiomanKilled, 1},
         Probability{tankDamageSystem::kTransmissionDestroyed, 0}, Probability{tankDamageSystem::kEngineDestroyed, 0}, Probability{tankDamageSystem::kTurretJammed, 6}, Probability{tankDamageSystem::kGunDestroyed, 1},
         Probability{tankDamageSystem::kScopeDamaged, 7}, Probability{tankDamageSystem::kCrewKilled, 0}, Probability{tankDamageSystem::kCrewShellShocked, 0}, Probability{tankDamageSystem::kRicochet, 24} };

    m_damageProbabilities[ThreatLevel{ -2 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 0}, Probability{tankDamageSystem::kBurning, 1}, Probability{tankDamageSystem::kTrackDamaged, 8}, Probability{tankDamageSystem::kDriverKilled, 2},
         Probability{tankDamageSystem::kCommanderKilled, 2}, Probability{tankDamageSystem::kLoaderKilled, 0}, Probability{tankDamageSystem::kGunnerKilled, 2}, Probability{tankDamageSystem::kRadiomanKilled, 2},
         Probability{tankDamageSystem::kTransmissionDestroyed, 2}, Probability{tankDamageSystem::kEngineDestroyed, 0}, Probability{tankDamageSystem::kTurretJammed, 5}, Probability{tankDamageSystem::kGunDestroyed, 2},
         Probability{tankDamageSystem::kScopeDamaged, 6}, Probability{tankDamageSystem::kCrewKilled, 0}, Probability{tankDamageSystem::kCrewShellShocked, 0}, Probability{tankDamageSystem::kRicochet, 22} };

    m_damageProbabilities[ThreatLevel{ -1 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 1}, Probability{tankDamageSystem::kBurning, 2}, Probability{tankDamageSystem::kTrackDamaged, 7}, Probability{tankDamageSystem::kDriverKilled, 2},
         Probability{tankDamageSystem::kCommanderKilled, 3}, Probability{tankDamageSystem::kLoaderKilled, 1}, Probability{tankDamageSystem::kGunnerKilled, 2}, Probability{tankDamageSystem::kRadiomanKilled, 2},
         Probability{tankDamageSystem::kTransmissionDestroyed, 2}, Probability{tankDamageSystem::kEngineDestroyed, 0}, Probability{tankDamageSystem::kTurretJammed, 4}, Probability{tankDamageSystem::kGunDestroyed, 3},
         Probability{tankDamageSystem::kScopeDamaged, 4}, Probability{tankDamageSystem::kCrewKilled, 4}, Probability{tankDamageSystem::kCrewShellShocked, 1}, Probability{tankDamageSystem::kRicochet, 19} };

    m_damageProbabilities[ThreatLevel{ 1 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 3}, Probability{tankDamageSystem::kBurning, 4}, Probability{tankDamageSystem::kTrackDamaged, 5}, Probability{tankDamageSystem::kDriverKilled, 3},
         Probability{tankDamageSystem::kCommanderKilled, 4}, Probability{tankDamageSystem::kLoaderKilled, 2}, Probability{tankDamageSystem::kGunnerKilled, 3}, Probability{tankDamageSystem::kRadiomanKilled, 2},
         Probability{tankDamageSystem::kTransmissionDestroyed, 3}, Probability{tankDamageSystem::kEngineDestroyed, 2}, Probability{tankDamageSystem::kTurretJammed, 2}, Probability{tankDamageSystem::kGunDestroyed, 3},
         Probability{tankDamageSystem::kScopeDamaged, 2}, Probability{tankDamageSystem::kCrewKilled, 3}, Probability{tankDamageSystem::kCrewShellShocked, 3}, Probability{tankDamageSystem::kRicochet, 11} };

    m_damageProbabilities[ThreatLevel{ 2 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 4}, Probability{tankDamageSystem::kBurning, 5}, Probability{tankDamageSystem::kTrackDamaged, 4}, Probability{tankDamageSystem::kDriverKilled, 4},
         Probability{tankDamageSystem::kCommanderKilled, 4}, Probability{tankDamageSystem::kLoaderKilled, 2}, Probability{tankDamageSystem::kGunnerKilled, 3}, Probability{tankDamageSystem::kRadiomanKilled, 2},
         Probability{tankDamageSystem::kTransmissionDestroyed, 3}, Probability{tankDamageSystem::kEngineDestroyed, 3}, Probability{tankDamageSystem::kTurretJammed, 1}, Probability{tankDamageSystem::kGunDestroyed, 2},
         Probability{tankDamageSystem::kScopeDamaged, 2}, Probability{tankDamageSystem::kCrewKilled, 4}, Probability{tankDamageSystem::kCrewShellShocked, 4}, Probability{tankDamageSystem::kRicochet, 8} };


    m_damageProbabilities[ThreatLevel{ 3 }] = std::vector{
         Probability{tankDamageSystem::kExploded, 5}, Probability{tankDamageSystem::kBurning, 6}, Probability{tankDamageSystem::kTrackDamaged, 3}, Probability{tankDamageSystem::kDriverKilled, 4},
         Probability{tankDamageSystem::kCommanderKilled, 5}, Probability{tankDamageSystem::kLoaderKilled, 3}, Probability{tankDamageSystem::kGunnerKilled, 3}, Probability{tankDamageSystem::kRadiomanKilled, 2},
         Probability{tankDamageSystem::kTransmissionDestroyed, 3}, Probability{tankDamageSystem::kEngineDestroyed, 3}, Probability{tankDamageSystem::kTurretJammed, 0}, Probability{tankDamageSystem::kGunDestroyed, 1},
         Probability{tankDamageSystem::kScopeDamaged, 1}, Probability{tankDamageSystem::kCrewKilled, 5}, Probability{tankDamageSystem::kCrewShellShocked, 5}, Probability{tankDamageSystem::kRicochet, 6} };




}
