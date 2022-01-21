
#include "damage_calculator.h"


namespace ranges =  std::ranges;

bool core::DamageCalculator::isTargerReachable(const Unit* sourceUnit, const Unit* targetUnit, const std::vector<GameTile>& lineOfFire) const
{
	if (TileDistance{ std::size(lineOfFire) } > sourceUnit->getRangeOfFire())
		return false;
	
	return ranges::all_of(lineOfFire, [](const GameTile& tile) { return tile.isShootableThrough(); });
}

int core::DamageCalculator::generateUniformRandNumber(const int rangeFrom, const int rangeTo)
{
	std::uniform_int_distribution  distribution(rangeFrom, rangeTo);

	return distribution(m_generator);
}

void core::DamageCalculator::initProbabilityTables()
{
	m_damageHandlers.insert({ typeid(TankUnit),  DamageProbabilityTable() });
	m_damageHandlers[typeid(TankUnit)].fillTankTableWithoutFile();
}

UnitShootInfo  core::DamageCalculator::shot( Unit* sourceUnit, Unit* targetUnit, const std::vector<GameTile>& lineOfFire) 
{
	UnitShootInfo info{ sourceUnit->getID(), targetUnit->getID() };
	info.m_damageDone = tank_state_system::kMissed;

	if(!isTargerReachable(sourceUnit, targetUnit, lineOfFire))
	{
		std::cout << "Target unit: " << targetUnit->getID().identifier << " is unreachable \n";
		return info;
	}

	Shots amountOfShots{ 1 };
	sourceUnit->shots(amountOfShots);
	HitThreshold currentThreshold = calculateHitThreshold(sourceUnit, targetUnit, lineOfFire);
	auto rolledNumber = HitThreshold{ rollDiceWithFaces() };
	std::cout << "Threshold: " << currentThreshold.hitThreshold << ", rolled: " << rolledNumber.hitThreshold << "\n";
	if (rolledNumber >= currentThreshold) // if rolled number is higher then targetUnit is hit
	{
		std::type_index typeIndx = std::type_index(typeid(*targetUnit));
		if (!m_damageHandlers.contains(typeIndx))
			std::cout << "ERRROR\n";// TODO throw custom exception
		
		Attack unitAttack = sourceUnit->getAttack();
		auto armor = targetUnit->getArmor(sourceUnit->getGunRotation());
		ThreatLevel threat{ unitAttack.attack  - armor };
		int overallProbabilitySize = m_damageHandlers.at(typeIndx).getOverallProbabilitySize(threat);
		int rolledNumber = rollDiceWithFaces(overallProbabilitySize);
		std::string_view destoyed_part = m_damageHandlers.at(typeIndx).getDestroyedPart(threat, rolledNumber);
		info.m_damageDone = destoyed_part;
		std::cout << "Threat lvl: " << threat.level << ", RolledNumber: " << rolledNumber << ", DAMAGE TYPE: " << destoyed_part << "\n";
	}
	else
	{
		std::cout << "SHOT MISSED\n";
	}

	return info;

}

core::HitThreshold core::DamageCalculator::calculateHitThreshold(const Unit* source, const Unit* target, const std::vector<GameTile>& lineOfFire) const
{
	// TODO create smarter handler for tile types which effects Hit Threshold number(it's gonna be tough to handle a lot of different tile types also violates solid)
	// TODO a lot of forfeits are missed rn, add all from the game rules
	auto isTileEffectsHitThreshold = [](const auto& tile) { return (tile.m_type == GameTileType::Shrub) ? true : false; };
	short obstaclesCount = ranges::count_if(begin(lineOfFire), end(lineOfFire) - 1, isTileEffectsHitThreshold);
	short finePerDistance = 5;
	HitThreshold fineForDistance(size(lineOfFire) / finePerDistance);
	HitThreshold totalThreshold = m_defaultHitThreshold + fineForDistance + HitThreshold{ obstaclesCount };

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