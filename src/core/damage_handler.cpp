#include "damage_handler.h"

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
		return tank_state_system::kRicochet;
}

void core::DamageProbabilityTable::fillTankTableWithoutFile()
{
	m_damageProbabilities[ThreatLevel{ 0 }] = std::vector{
		 Probability{tank_state_system::kExploded, 2}, Probability{tank_state_system::kBurning, 3}, Probability{tank_state_system::kTrackDamaged, 6}, Probability{tank_state_system::kDriverKilled, 3},
		 Probability{tank_state_system::kCommanderKilled, 3}, Probability{tank_state_system::kLoaderKilled, 1}, Probability{tank_state_system::kGunnerKilled, 3}, Probability{tank_state_system::kRadiomanKilled, 2},
		 Probability{tank_state_system::kTransmissionDestroyed, 2}, Probability{tank_state_system::kEngineDestroyed, 1}, Probability{tank_state_system::kTurretJammed, 4}, Probability{tank_state_system::kGunDestroyed, 4},
		 Probability{tank_state_system::kScopeDamaged, 3}, Probability{tank_state_system::kCrewKilled, 2}, Probability{tank_state_system::kCrewShellShocked, 2}, Probability{tank_state_system::kRicochet, 15} };

	m_damageProbabilities[ThreatLevel{ -3 }] = std::vector{
		 Probability{tank_state_system::kExploded, 0}, Probability{tank_state_system::kBurning, 0}, Probability{tank_state_system::kTrackDamaged, 9}, Probability{tank_state_system::kDriverKilled, 1},
		 Probability{tank_state_system::kCommanderKilled, 2}, Probability{tank_state_system::kLoaderKilled, 0}, Probability{tank_state_system::kGunnerKilled, 1}, Probability{tank_state_system::kRadiomanKilled, 1},
		 Probability{tank_state_system::kTransmissionDestroyed, 0}, Probability{tank_state_system::kEngineDestroyed, 0}, Probability{tank_state_system::kTurretJammed, 6}, Probability{tank_state_system::kGunDestroyed, 1},
		 Probability{tank_state_system::kScopeDamaged, 7}, Probability{tank_state_system::kCrewKilled, 0}, Probability{tank_state_system::kCrewShellShocked, 0}, Probability{tank_state_system::kRicochet, 24} };

	m_damageProbabilities[ThreatLevel{ -2 }] = std::vector{
		 Probability{tank_state_system::kExploded, 0}, Probability{tank_state_system::kBurning, 1}, Probability{tank_state_system::kTrackDamaged, 8}, Probability{tank_state_system::kDriverKilled, 2},
		 Probability{tank_state_system::kCommanderKilled, 2}, Probability{tank_state_system::kLoaderKilled, 0}, Probability{tank_state_system::kGunnerKilled, 2}, Probability{tank_state_system::kRadiomanKilled, 2},
		 Probability{tank_state_system::kTransmissionDestroyed, 2}, Probability{tank_state_system::kEngineDestroyed, 0}, Probability{tank_state_system::kTurretJammed, 5}, Probability{tank_state_system::kGunDestroyed, 2},
		 Probability{tank_state_system::kScopeDamaged, 6}, Probability{tank_state_system::kCrewKilled, 0}, Probability{tank_state_system::kCrewShellShocked, 0}, Probability{tank_state_system::kRicochet, 22} };

	m_damageProbabilities[ThreatLevel{ -1 }] = std::vector{
		 Probability{tank_state_system::kExploded, 1}, Probability{tank_state_system::kBurning, 2}, Probability{tank_state_system::kTrackDamaged, 7}, Probability{tank_state_system::kDriverKilled, 2},
		 Probability{tank_state_system::kCommanderKilled, 3}, Probability{tank_state_system::kLoaderKilled, 1}, Probability{tank_state_system::kGunnerKilled, 2}, Probability{tank_state_system::kRadiomanKilled, 2},
		 Probability{tank_state_system::kTransmissionDestroyed, 2}, Probability{tank_state_system::kEngineDestroyed, 0}, Probability{tank_state_system::kTurretJammed, 4}, Probability{tank_state_system::kGunDestroyed, 3},
		 Probability{tank_state_system::kScopeDamaged, 4}, Probability{tank_state_system::kCrewKilled, 4}, Probability{tank_state_system::kCrewShellShocked, 1}, Probability{tank_state_system::kRicochet, 19} };

	m_damageProbabilities[ThreatLevel{ 1 }] = std::vector{
		 Probability{tank_state_system::kExploded, 3}, Probability{tank_state_system::kBurning, 4}, Probability{tank_state_system::kTrackDamaged, 5}, Probability{tank_state_system::kDriverKilled, 3},
		 Probability{tank_state_system::kCommanderKilled, 4}, Probability{tank_state_system::kLoaderKilled, 2}, Probability{tank_state_system::kGunnerKilled, 3}, Probability{tank_state_system::kRadiomanKilled, 2},
		 Probability{tank_state_system::kTransmissionDestroyed, 3}, Probability{tank_state_system::kEngineDestroyed, 2}, Probability{tank_state_system::kTurretJammed, 2}, Probability{tank_state_system::kGunDestroyed, 3},
		 Probability{tank_state_system::kScopeDamaged, 2}, Probability{tank_state_system::kCrewKilled, 3}, Probability{tank_state_system::kCrewShellShocked, 3}, Probability{tank_state_system::kRicochet, 11} };

	m_damageProbabilities[ThreatLevel{ 2 }] = std::vector{
		 Probability{tank_state_system::kExploded, 4}, Probability{tank_state_system::kBurning, 5}, Probability{tank_state_system::kTrackDamaged, 4}, Probability{tank_state_system::kDriverKilled, 4},
		 Probability{tank_state_system::kCommanderKilled, 4}, Probability{tank_state_system::kLoaderKilled, 2}, Probability{tank_state_system::kGunnerKilled, 3}, Probability{tank_state_system::kRadiomanKilled, 2},
		 Probability{tank_state_system::kTransmissionDestroyed, 3}, Probability{tank_state_system::kEngineDestroyed, 3}, Probability{tank_state_system::kTurretJammed, 1}, Probability{tank_state_system::kGunDestroyed, 2},
		 Probability{tank_state_system::kScopeDamaged, 2}, Probability{tank_state_system::kCrewKilled, 4}, Probability{tank_state_system::kCrewShellShocked, 4}, Probability{tank_state_system::kRicochet, 8} };


	m_damageProbabilities[ThreatLevel{ 3 }] = std::vector{
		 Probability{tank_state_system::kExploded, 5}, Probability{tank_state_system::kBurning, 6}, Probability{tank_state_system::kTrackDamaged, 3}, Probability{tank_state_system::kDriverKilled, 4},
		 Probability{tank_state_system::kCommanderKilled, 5}, Probability{tank_state_system::kLoaderKilled, 3}, Probability{tank_state_system::kGunnerKilled, 3}, Probability{tank_state_system::kRadiomanKilled, 2},
		 Probability{tank_state_system::kTransmissionDestroyed, 3}, Probability{tank_state_system::kEngineDestroyed, 3}, Probability{tank_state_system::kTurretJammed, 0}, Probability{tank_state_system::kGunDestroyed, 1},
		 Probability{tank_state_system::kScopeDamaged, 1}, Probability{tank_state_system::kCrewKilled, 5}, Probability{tank_state_system::kCrewShellShocked, 5}, Probability{tank_state_system::kRicochet, 6} };




}
