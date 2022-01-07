#include "damage_handler.h"

#include <iostream>


core::Probability core::ProbabilitiesOfDamage::getProbability(const ThreatLevel& lvl)
{
	if (m_damageProbabilites.contains(lvl))
		return m_damageProbabilites[lvl];
	else
		return Probability{ 0 };
}

void core::ProbabilitiesOfDamage::setProbability(const ThreatLevel& lvl, const Probability& probability)
{
	m_damageProbabilites[lvl] = probability;
}

void core::DamageHandler::loadProbabilitiesFromFile(std::string_view filename)
{
}

void core::DamageHandler::calcEvent(const ThreatLevel& lvl)
{
	std::cout << "calcEvent\n";
}

core::TankDamageHandler::TankDamageHandler()
{
	for (const auto& type : tank_damage_system::kTankDamageTypes)
	{
		// here we should load from file or add table with probabilities
		// Example: engineDamaged->level(marker)->probability
		m_damageProbabilities.insert({ type, ProbabilitiesOfDamage{} });
	}
	//
}
