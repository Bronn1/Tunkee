#include "damage_handler.h"



core::Probability core::ProbabilitiesOfDamage::getProbability(const DamageLevel& lvl)
{
	if (m_damageProbabilites.contains(lvl))
		return m_damageProbabilites[lvl];
	else
		return Probability{ 0 };
}

void core::ProbabilitiesOfDamage::setProbability(const DamageLevel& lvl, const Probability& probability)
{
	m_damageProbabilites[lvl] = probability;
}

void core::DamageHandler::loadProbabilitiesFromFile(std::string_view filename)
{
}

void core::DamageHandler::calcEvent(const DamageLevel& lvl)
{
}

core::TankDamageHandler::TankDamageHandler()
{
	for (const auto& type : kTankDamageType)
	{
		// here we should load from file or add table with probabilities
		// Example: engineDamaged->level(marker)->probability
		m_damageProbabilities.insert({ type, ProbabilitiesOfDamage{} });
	}
	//
}
