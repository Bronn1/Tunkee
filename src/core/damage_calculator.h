#pragma once

#include "data_types.h"
#include "unit.h"
#include "damage_handler.h"
#include "game_tile.h"

#include <typeindex>  
#include <unordered_map>
#include <memory>
#include <functional>


namespace core {
	constexpr int kMaxHitThreshold = 10;
	constexpr int kMinHitThreshold = 0;

	struct HitThreshold {
		explicit HitThreshold( int  val) : hitThreshold(val) {
			if (hitThreshold > kMaxHitThreshold) hitThreshold = kMaxHitThreshold;
			if (hitThreshold < kMinHitThreshold) hitThreshold = kMinHitThreshold;
		}
		int hitThreshold{ 2 };
		auto operator+(const HitThreshold& other) const { return HitThreshold(hitThreshold + other.hitThreshold); }
		auto operator-(const HitThreshold& other) const { return HitThreshold(hitThreshold - other.hitThreshold); }
		auto& operator+=(const HitThreshold& other);
		auto& operator-=(const HitThreshold& other);
		auto operator<=>(const HitThreshold& other) const  = default;
	};

	class DamageCalculator 
	{
	public:
		void initProbabilityTables();
		ShootUnitInfo shot( Unit* source,  Unit* target, const std::vector<GameTile>& lineOfFire) const ;
		HitThreshold calculateHitThreshold(const Unit* sourceUnit, const Unit* targetUnit, const std::vector<GameTile>& lineOfFire) const;
		bool isTargerReachable(const Unit* sourceUnit, const Unit* targetUnit, const std::vector<GameTile>& lineOfFire) const;
		int generateUniformRandNumber(const int rangeFrom, const int rangeTo) const ;
		int rollDiceWithFaces(int amountOfFaces = kMaxHitThreshold) const;
	private:
		
		/** @brief holds Probability tables of damgage based on unit type */
		std::unordered_map<std::type_index, DamageProbabilityTable> m_damageHandlers;
		HitThreshold m_defaultHitThreshold{ 2 };
	};
}

