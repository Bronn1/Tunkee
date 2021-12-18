#pragma once

#include "unit_damage_type.h"

#include <unordered_map>
#include <functional>

namespace core {
	constexpr  int kMaxLevel = 3;
	constexpr  int kMinLevel = -3;

	struct DamageLevel
	{
		DamageLevel(int lvl)
		{
			if (lvl > kMaxLevel) lvl = kMaxLevel;
			if (lvl < kMinLevel) lvl = kMinLevel;
			level = lvl;

		}
		bool operator==(const DamageLevel&) const = default;
		auto operator<=>(const DamageLevel&) const = default;

		int level;
	};

	struct Probability {
		float probability;
	};

	struct DamageLevelHasher
	{
		size_t operator()(const DamageLevel& level) const
		{
			return (std::hash<int>()(level.level));
		}
	};

	/**
	 * @brief Holds information about probabilites of taking damage under
	 * different levels. Level is a difference between shot power of attacking unit
	 * and armor of defending unit
	*/
	struct ProbabilitiesOfDamage {
	public:
		//ProbabilitiesOfDamage() { std::cout << "damageDDDDDD\n"; }
		Probability getProbability(const DamageLevel& lvl);
		void setProbability(const DamageLevel& lvl, const Probability& probability);
	private:
		std::unordered_map<DamageLevel, Probability, DamageLevelHasher> m_damageProbabilites{};
	};

	using UnitDamageType = std::string_view;
	class DamageHandler {
	public:
		void loadProbabilitiesFromFile(std::string_view filename);
		void calcEvent(const DamageLevel& lvl);
		virtual ~DamageHandler() = default;
	protected:
		std::unordered_map<UnitDamageType, ProbabilitiesOfDamage> m_damageProbabilities;

	};

	class TankDamageHandler : public DamageHandler {
	public:
		TankDamageHandler();
	};
}
