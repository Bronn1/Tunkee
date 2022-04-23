
#include "damage_calculator.h"


namespace ranges =  std::ranges;

bool core::DamageCalculator::isTargerReachable(const Unit* sourceUnit, const Unit* targetUnit, const std::vector<GameTile>& lineOfFire) const
{
    if (TileDistance{ static_cast<unsigned>(std::size(lineOfFire)) } > sourceUnit->getRangeOfFire())
        return false;
    
    return ranges::all_of(lineOfFire, [](const GameTile& tile) { return tile.isShootableThrough(); });
}

int core::DamageCalculator::generateUniformRandNumber(const int rangeFrom, const int rangeTo) const
{
    std::uniform_int_distribution  distribution(rangeFrom, rangeTo);

    return distribution(m_generator);
}

std::vector<UnitStateInfo> core::DamageCalculator::nextTurn()
{
    std::vector<UnitStateInfo> changedUnitStates;
    std::set<Unit*> tmp{};
    //move these constans later
    constexpr int kChanceToStopBurning = 5;
    constexpr  int kChanceToExplode = 8;
    constexpr  int kChanceToKeepBurning = 10;
    for (auto& unit : m_burningUnits)
    {
        int rolledNumber = rollDiceWithFaces(kChanceToKeepBurning);
        if (rolledNumber <= kChanceToStopBurning)
        {
            UnitStateInfo info{ UnitIdentifier{0}, unit->getID(), DamageStatus::Normal };
            info.m_damageTypeName = tankDamageSystem::kBurning;
            unit->setDamageStatus(tankDamageSystem::kBurning, DamageStatus::Normal);
            changedUnitStates.push_back(info);
        }
        else if (rolledNumber <= kChanceToExplode)
        {
            UnitStateInfo info{ UnitIdentifier{0}, unit->getID(), DamageStatus::Damaged };
            info.m_damageTypeName = tankDamageSystem::kExploded;
            changedUnitStates.push_back(info);
            unit->applyDamage(tankDamageSystem::kExploded);
        }
        else
            tmp.insert(unit);
    }
    m_burningUnits = std::move(tmp);

    return changedUnitStates;
}

void core::DamageCalculator::saveTempDamage(Unit* targetUnit, const DamageTo damageTo)
{
    if (damageTo == tankDamageSystem::kBurning)
        m_burningUnits.insert(targetUnit);
}

void core::DamageCalculator::fillDamageProbabilities(UnitPartsInfoVec& partsInfoVec, const  Unit* selectedUnit, const Unit* targetUnit, const Angle& requiredGunAngleToShot) const
{
    //std::remove_const<decltype(e)>::type
    std::type_index typeIndx = std::type_index(typeid(*targetUnit));
    if (!m_damageTables.contains(typeIndx))
        throw std::runtime_error(std::string("Unknown unit type received in damage calculator") + typeid(targetUnit).name());// TODO throw custom exception

    Attack unitAttack = selectedUnit->getAttack();
    auto armor = targetUnit->getArmor(requiredGunAngleToShot);
    ThreatLevel threat{ unitAttack.attack - armor };
    int overallProbSize = m_damageTables.at(typeIndx).getOverallProbabilitySize(threat);
    const int kMaxPercent = 100;
    for (auto& partInfo : partsInfoVec)
    {
        int probToHitPart = m_damageTables.at(typeIndx).getProbabilityTo(threat, partInfo.m_name);
        partInfo.chanceToHitPercent = (probToHitPart * kMaxPercent) / overallProbSize;
    }
}

void core::DamageCalculator::initProbabilityTables()
{
    m_damageTables.insert({ typeid(TankUnit),  DamageProbabilityTable() });
    m_damageTables[typeid(TankUnit)].fillTankTableWithoutFile();
}

UnitStateInfo  core::DamageCalculator::shot( Unit* sourceUnit, Unit* targetUnit, const std::vector<GameTile>& lineOfFire)
{
    UnitStateInfo info{ sourceUnit->getID(), targetUnit->getID(), DamageStatus::Damaged };
    info.m_damageTypeName = core::kShotMissed;

    if(!isTargerReachable(sourceUnit, targetUnit, lineOfFire))
    {
        std::cout << "Target unit: " << targetUnit->getID().identifier << " is unreachable \n";
        return info;
    }

    Shots amountOfShots{ 1 };
    sourceUnit->shoot(amountOfShots);
    HitThreshold currentThreshold = calculateHitThreshold(sourceUnit, targetUnit, lineOfFire);
    auto rolledNumber = HitThreshold{ rollDiceWithFaces() };
    std::cout << "Threshold: " << currentThreshold.hitThreshold << ", rolled: " << rolledNumber.hitThreshold << "\n";
    if (rolledNumber >= currentThreshold) // if rolled number is higher then targetUnit is hit
    {
        std::type_index typeIndx = std::type_index(typeid(*targetUnit));
        if (!m_damageTables.contains(typeIndx))
            throw std::runtime_error(std::string("Unknown unit type received in damage calculator") + typeid(targetUnit).name());
        
        Attack unitAttack = sourceUnit->getAttack();
        auto armor = targetUnit->getArmor(sourceUnit->getGunRotation());
        ThreatLevel threat{ unitAttack.attack  - armor };
        int overallProbabilitySize = m_damageTables.at(typeIndx).getOverallProbabilitySize(threat);
        int rolledNumber = rollDiceWithFaces(overallProbabilitySize);
        std::string_view destoyedPart = m_damageTables.at(typeIndx).getDestroyedPart(threat, rolledNumber);
        targetUnit->applyDamage(destoyedPart);
        info.m_damageTypeName = destoyedPart;
        saveTempDamage(targetUnit, destoyedPart);
        std::cout << "Threat lvl: " << threat.level << ", RolledNumber: " << rolledNumber << ", DAMAGE TYPE: " << destoyedPart << "\n";
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
    // TODO a lot of fine are missed rn, add all from the game rules
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