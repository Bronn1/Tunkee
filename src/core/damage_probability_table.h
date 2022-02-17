#pragma once

#include "unit_damage_system_strategy.h"

#include <unordered_map>
#include <functional>
#include <numeric>

namespace core {
	constexpr  int kMaxLevel = 3;
	constexpr  int kMinLevel = -3;

	/**
    * @brief Level is a difference between shot power of attacking unit
    * and armor of defending unit
    */
	struct ThreatLevel
	{
		explicit ThreatLevel(int lvl)
		{
			if (lvl > kMaxLevel) lvl = kMaxLevel;
			if (lvl < kMinLevel) lvl = kMinLevel;
			level = lvl;

		}
		// 270 +- 60 front else flank
		bool operator==(const ThreatLevel&) const = default;
		auto operator<=>(const ThreatLevel&) const = default;

		int level;
	};

	using UnitDamageType = std::string_view;

	struct Probability {
		UnitDamageType type;
		int probability;
	};

	struct ThreatLevelHasher
	{
		size_t operator()(const ThreatLevel& level) const
		{
			return (std::hash<int>()(level.level));
		}
	};

	
	using ProbabilitesVec = std::vector<Probability> ;
	
	class DamageProbabilityTable {
	public:
		void loadProbabilitiesFromFile(std::string_view filename);
		void calcEvent(const ThreatLevel& lvl);
		int getOverallProbabilitySize(const ThreatLevel& lvl) const;
		std::string_view getDestroyedPart(const ThreatLevel& lvl, int rolledProbability) const;
		void fillTankTableWithoutFile();
	protected:
		std::unordered_map<ThreatLevel, ProbabilitesVec, ThreatLevelHasher> m_damageProbabilities;

	};
}
