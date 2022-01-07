
#include "damage_calculator.h"

#include <random>
#include <typeinfo>    

namespace ranges =  std::ranges;

core::DamageCalculator::DamageCalculator()
{
	m_damageHandlers.insert({ typeid(TankUnit), std::make_unique<TankDamageHandler>() });
}

bool core::DamageCalculator::isTargerReachable(const Unit* sourceUnit, const Unit* targetUnit, const std::vector<GameTile>& lineOfFire) const
{
	if (TileDistance{ std::size(lineOfFire) } > sourceUnit->getRangeOfFire())
		return false;
	
	return ranges::all_of(lineOfFire, [](const GameTile& tile) { return tile.isShootableThrough(); });
}

int core::DamageCalculator::rollDiceWithFaces(int amountOfFaces) const
{
	return generateUniformRandNumber(kMinHitThreshold, amountOfFaces);
}

int core::DamageCalculator::generateUniformRandNumber(const int rangeFrom, const int rangeTo) const
{
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_int_distribution  distribution(rangeFrom, rangeTo);

	return distribution(generator);
}

bool core::DamageCalculator::shot( Unit* sourceUnit, Unit* targetUnit, const std::vector<GameTile>& lineOfFire) const
{
	if(!isTargerReachable(sourceUnit, targetUnit, lineOfFire))
	{
		std::cout << "Target unit: " << targetUnit->getID().identifier << " is unreachable \n";
		return false;
	}
	Shots amountOfShots{ 1 };
	sourceUnit->shots(amountOfShots);
	HitThreshold currentThreshold = calculateHitThreshold(sourceUnit, targetUnit, lineOfFire);
	HitThreshold rolledNumber = rollDiceWithFaces();
	std::cout << "threshold: " << currentThreshold.hitThreshold << ", rolled: " << rolledNumber.hitThreshold << "\n";

	
	if (rolledNumber >= currentThreshold) // if rolled number is higher then targetUnit is hit
	{
		std::type_index typeIndx = std::type_index(typeid(*targetUnit));
		if (!m_damageHandlers.contains(typeIndx))
			std::cout << "ERRROR\n";// TODO throw custom exception
		
		ThreatLevel thread{ 5 - 1 };
		m_damageHandlers.at(typeIndx)->calcEvent(thread);
	}
	else
	{
		std::cout << "SHOT MISSED\n";
	}

	return false;

}

core::HitThreshold core::DamageCalculator::calculateHitThreshold(const Unit* source, const Unit* target, const std::vector<GameTile>& lineOfFire) const
{
	// TODO create smarter handler for tile types which effects Hit Threshold number(it's gonna be tough to handle a lot of different tile types also violates solid)
	// TODO a lot of forfeits are missed rn, add all from the game rules
	auto isTileEffectsHitThreshold = [](const auto& tile) { return (tile.m_type == GameTileType::Shrub) ? true : false; };
	short obstaclesCount = ranges::count_if(begin(lineOfFire), end(lineOfFire) - 1, isTileEffectsHitThreshold);
	short forfeitPerDistance = 5;
	HitThreshold forfeitForDistance(size(lineOfFire) / forfeitPerDistance);
	HitThreshold totalThreshold = m_defaultHitThreshold + forfeitForDistance + HitThreshold{ obstaclesCount };

	return totalThreshold;
}

auto& core::HitThreshold::operator+=(const HitThreshold& other) 
{
	hitThreshold += other.hitThreshold;
	if (hitThreshold > kMaxHitThreshold) hitThreshold = kMaxHitThreshold;
	if (hitThreshold < kMinHitThreshold) hitThreshold = kMinHitThreshold;
	return *this;
}

auto& core::HitThreshold::operator-=(const HitThreshold& other)
{
	hitThreshold -= other.hitThreshold;
	if (hitThreshold > kMaxHitThreshold) hitThreshold = kMaxHitThreshold;
	if (hitThreshold < kMinHitThreshold) hitThreshold = kMinHitThreshold;
	return *this;
}