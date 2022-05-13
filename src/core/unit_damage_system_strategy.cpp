#include "unit_damage_system_strategy.h"

#include <cmath>
#include <ranges>


namespace  ranges = std::ranges;

void tankDamageSystem::TankDamageSystemStrategy::applyDamage(const std::string_view damageType, ActionStatus actionStatus)
{
    m_crew.applyDamage(damageType, DamageStatus::Damaged);
    setActionMiss(damageType, actionStatus);

    if (damageType == kExploded || damageType == kCrewKilled)
        m_crew.killAll();

    if (m_damageableParts.contains(damageType))
        m_damageableParts.at(damageType).applyDamage();

    checkAliveSystemsAfterDamage();
    if (!isDamageVisibleForEnemy(damageType))
        ++m_hiddenDamageCounter;
}

void tankDamageSystem::TankDamageSystemStrategy::checkAliveSystemsAfterDamage()
{
    if (m_crew.getAliveMembersCount() == 0)
    {
        m_isAlive = false;
        m_isAliveForEnemy = (!m_crew.isVisibleForEnemy()) ? true : false;
    }

    if (m_damageableParts[kExploded].getState() == Damaged)
    {
        m_isAlive = false;
        m_isAliveForEnemy = false;
    }

    if(m_damageableParts[kEngine].getState() == Damaged && m_damageableParts[kGunDestroyed].getState() == Damaged)
        m_isAlive = false;
}

bool tankDamageSystem::TankDamageSystemStrategy::isDamageVisibleForEnemy(const DamageTo damageType) const
{
    if (m_damageableParts.contains(damageType))
        return m_damageableParts.at(damageType).isVisibleForEnemy();
    if(m_crew.contains(damageType))
        return m_crew.isVisibleForEnemy();

    return true;
}

 


// "ScopeDamaged" - ����� � ����������� ���������( +1 ��� +2). ����� ������� ������ -1 � ��������� 
//  "TrackDamaged" - ������ ���� ������ ���������, ����������. �����
// 
// 
// "Burning"  - ����� ���� - ����������� ���� ������� ���������, �������, ���������� ������.  (�������������� �������� ������ �����?). ����������� ��� �����
//  "TurretJammed"  - turret can't rotate
// "Exploded" - ��������� ����� ����� ����� 
// "CommanderKilled" - ? (����������� �������� ��������) 
// "DriverKilled" -  ? � ���� ��� ����� �� �����,����� �� ��� �� ������ ��� �����? ������������� -1 ������� � �����
// "loaderKilled" -  ? (����� ������� ���� ���� �������, �� ����� ���� ��������, ���� ������. 
// ���� ����� ������ � �����, �� ������ � ���������������, (loader, gunner, Driver)               round up ((rate of fire* ������� � �����) / 3) 
// "gunnerKilled" - ?   
// �������� ���� ������� � ��������� ������?
// "TransmissionDestroyed"  - movement = 1;
// "EngineDestroyed" cant move immobile
// "GunDestroyed" - can't fire (invisible),ai ���� ������ �� ������. � �������� ������� �����, ����� ����������, ��� �� �����������? .
// "CrewKilled" - ���� ���������, �� ������ �������
// "CrewShellShocked" - ������� ������ ����,  ������ �������. 
// �������� ������� ������� � ����� � ����� ���-��
//  range - 20 
// ���������� � ��� �� �������, � ���������� damage

tankDamageSystem::TankDamageSystemStrategy::TankDamageSystemStrategy(CrewInfo info)
{
    m_crew.initCrew(std::move(info));
    bool isDamageVisibleForEnemy = true;
    m_damageableParts[kBurning] = UnitPart(isDamageVisibleForEnemy);
    m_damageableParts[kExploded] = UnitPart(isDamageVisibleForEnemy);
    m_damageableParts[kTransmission] = UnitPart();
    m_damageableParts[kEngine] = UnitPart();
    m_damageableParts[kTurretJammed] = UnitPart();
    m_damageableParts[kGunDestroyed] = UnitPart();
    m_damageableParts[kScope] = UnitPart();
    m_damageableParts[kTrack] = UnitPart();
    m_damageableParts[kCrewShellShocked] = UnitPart();
    m_damageableParts[kCrewKilled] = UnitPart();
}

TileDistance  tankDamageSystem::TankDamageSystemStrategy::getMoveDistanceWithFine(const TileDistance movement) const
{
    //maybe better to add vars to unit parts can move after damage and can shoot after damage , it will reduce amount of if's in code
    if (!isMovingSystemsAlive())
        return TileDistance{ 0 };

    if (m_damageableParts.at(kTransmission).getState() == Damaged) 
        return TileDistance{ 1 };

    return movement;
}

Shots  tankDamageSystem::TankDamageSystemStrategy::getRateOfFireWithFine(const Shots rateOfFire) const
{
    if (!isGunsAlive())
        return Shots{0};

    if (m_crew.getAliveGunMembersCount() <= 0 || m_crew.getAliveMembersCount() == 1)
        return Shots{ 1 };

    const unsigned kStandardGunCrew = 3;
    // round up ((rate of fire* Gun members left) / 3) 
    return Shots{ ((rateOfFire.shots * m_crew.getAliveGunMembersCount()) % kStandardGunCrew == 0) ? (rateOfFire.shots * m_crew.getAliveGunMembersCount()) / kStandardGunCrew  :
                                                                                  (rateOfFire.shots * m_crew.getAliveGunMembersCount()) / kStandardGunCrew + 1};
}

int  tankDamageSystem::TankDamageSystemStrategy::amountOfActionCanDo() const
{
    return (m_crew.getAliveMembersCount() > 1) ? 2 :  1;
}

bool  tankDamageSystem::TankDamageSystemStrategy::isMovingSystemsAlive() const
{ 
    if (m_crew.getAliveMembersCount() <= 0 || m_damageableParts.at(kEngine).getState() == Damaged || 
        m_missingAction.at(kDriver) == ActionMiss::currentTurn || m_missingAction.at(kCrewShellShocked) == ActionMiss::currentTurn)
        return false;
    
    return true;
}

bool tankDamageSystem::TankDamageSystemStrategy::isGunsAlive() const
{
    if (m_crew.getAliveMembersCount() <= 0 ||  m_damageableParts.at(kGunDestroyed).getState() == Damaged || m_missingAction.at(kCrewShellShocked) == ActionMiss::currentTurn)
        return false;

    return true;
}

bool tankDamageSystem::TankDamageSystemStrategy::isAlive(PointOfView pointOfView) const
{
    if (pointOfView == PointOfView::Enemy) return m_isAliveForEnemy;

    return m_isAlive;
}

void tankDamageSystem::TankDamageSystemStrategy::setDamageStatus(const DamageTo damageType, const DamageStatus status)
{
    m_crew.applyDamage(damageType, status);
    if (!m_damageableParts.contains(damageType)) return;

    m_damageableParts.at(damageType).setState(status);
}

void tankDamageSystem::TankDamageSystemStrategy::nextTurn()
{
    resetMissActionOnNextTurn();
}

void tankDamageSystem::TankDamageSystemStrategy::setDamageVisibleForEnemy(const std::vector<DamageTo>& damageNames)
{
    if (std::size(damageNames) == 0)
    {
        m_crew.setVisibleForEnemy(true);
        for (auto& [name, part] : m_damageableParts)
            part.setVisibilityForEnemy(true);
        m_isAliveForEnemy = m_isAlive;
    }
    else
    {
        for (auto& name : damageNames)
        {
            if (name == kCommander)
                m_crew.setVisibleForEnemy(true);
            if (m_damageableParts.contains(name))
                m_damageableParts.at(name).setVisibilityForEnemy(true);
        }
    }
        
}

void tankDamageSystem::TankDamageSystemStrategy::setActionMiss(const std::string_view damageType, ActionStatus actionStatus)
{
    if (m_missingAction.contains(damageType))
    {
        m_missingAction[damageType] = (static_cast<bool>(actionStatus)) ? ActionMiss::currentTurn : ActionMiss::nextTurn;
    }
}

void tankDamageSystem::TankDamageSystemStrategy::resetMissActionOnNextTurn()
{
    for (auto& [name, status] : m_missingAction)
    {
        if (status == ActionMiss::currentTurn) status = ActionMiss::none;
        else if (status == ActionMiss::nextTurn) status = ActionMiss::currentTurn;
    }
    //m_missAction = m_missActionOnNextTurn ^ false; // bitwise XOR to avoid extra branching (false ^ false = false)
}

core::UnitPartsInfoVec tankDamageSystem::TankDamageSystemStrategy::getPartsInfo() const
{
    UnitPartsInfoVec resInfo{};
    for (const auto& [name, part] : m_damageableParts)
    {
        resInfo.push_back(UnitPartInfo{ name , part.getState(), 0, part.isVisibleForEnemy()});
    }

    return resInfo;
}
