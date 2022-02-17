#include "unit_damage_system_strategy.h"

#include <cmath>

void tankDamageSystem::TankDamageSystemStrategy::applyDamage(const std::string_view damageType, ActionStatus actionStatus)
{
	if (!m_damageableParts.contains(damageType))
	{
		std::cout << "Unexpected damage type is recieved:" << damageType << "\n"; // TODO throw custom exception
		return;
	}

	m_damageableParts.at(damageType).applyDamage();
	m_damageableParts.at(damageType).setActionMissAccordingToStatus(actionStatus);
	m_crew -= m_damageableParts.at(damageType).getAndResetCrewEffect();
	checkAliveSystemsAfterDamage();
}

void tankDamageSystem::TankDamageSystemStrategy::checkAliveSystemsAfterDamage()
{
	if (!m_crew)
	{
		m_damageableParts[kLoaderKilled].setState(Damaged);
		m_damageableParts[kCommanderKilled].setState(Damaged);
		m_damageableParts[kDriverKilled].setState(Damaged);
		m_damageableParts[kRadiomanKilled].setState(Damaged);
		m_damageableParts[kGunnerKilled].setState(Damaged);
		m_isAlive = false;
		m_isAliveForEnemy = (!m_damageableParts[kCrewKilled].isVisibleForEnemy()) ? true : false;
	}

	if (m_damageableParts[kExploded].getState() == Damaged) 
		m_isAliveForEnemy = false;
}

 
// "Burning"  - ����� ���� - ����������� ���� ������� ���������, �������, ���������� ������.  (�������������� �������� ������ �����?). ����������� ��� �����

// "ScopeDamaged" - ����� � ����������� ���������( +1 ��� +2). ����� ������� ������ -1 � ��������� 
//  "TrackDamaged" - ������ ���� ������ ���������, ����������. �����
// 
// 
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
// TODO add in pathingding function to move only in one directions, so before findShortestPAth will be check if clicked tile belongs to this direction;
// unit recieve move area and path. and cut everything, or better unit will decide which function to pick


tankDamageSystem::TankDamageSystemStrategy::TankDamageSystemStrategy(const Crew& crew)
{
	setCrew(crew);
	m_damageableParts[kBurning] = UnitPart();
	m_damageableParts[kExploded] = UnitPart(Crew(999, 999));// Crew in constructor is how damage to this part effects crew count
	m_damageableParts[kCommanderKilled] = UnitPart(Crew(1, 0));
	m_damageableParts[kDriverKilled] = UnitPart(Crew(1, 1), UnitPart::ActionEffectAfterDamage::MissAction);
	m_damageableParts[kRadiomanKilled] = UnitPart(Crew(1, 0));
	m_damageableParts[kGunnerKilled] = UnitPart(Crew(1, 1));
	m_damageableParts[kLoaderKilled] = UnitPart(Crew(1, 1));
	m_damageableParts[kTransmissionDestroyed] = UnitPart();
	m_damageableParts[kEngineDestroyed] = UnitPart();
	m_damageableParts[kTurretJammed] = UnitPart();
	m_damageableParts[kGunDestroyed] = UnitPart();
	m_damageableParts[kScopeDamaged] = UnitPart();
	m_damageableParts[kTrackDamaged] = UnitPart();
	m_damageableParts[kCrewShellShocked] = UnitPart(Crew(1, 0), UnitPart::ActionEffectAfterDamage::MissAction);
	m_damageableParts[kCrewKilled] = UnitPart(Crew(999, 999));

}

TileDistance  tankDamageSystem::TankDamageSystemStrategy::getMoveDistanceWithFine(const TileDistance movement) const
{
	//maybe better to add vars to unit parts can move after damage and can shoot after damage , it will reduce amount of if's in code
	if (m_crew.overallCrewCount <= 0 || m_damageableParts.at(kEngineDestroyed).getState()== Damaged) 
		return TileDistance{ 0 };

	if (m_damageableParts.at(kDriverKilled).isMissAction() || m_damageableParts.at(kCrewShellShocked).isMissAction()) 
		return TileDistance{ 0 };

	if (m_damageableParts.at(kTransmissionDestroyed).getState() == Damaged) 
		return TileDistance{ 1 };

	return movement;
}

Shots  tankDamageSystem::TankDamageSystemStrategy::getRateOfFireWithFine(const Shots rateOfFire) const
{
	if (m_crew.overallCrewCount <= 0 || m_damageableParts.at(kGunDestroyed).getState() == Damaged || m_damageableParts.at(kCrewShellShocked).isMissAction()) 
		return Shots{0};

	if (m_crew.gunCrew <= 0 || m_crew.overallCrewCount == 1)
		return Shots{ 1 };

	const unsigned kStandardGunCrew = 3;
	// cant use std::ceil with unsigned 
	return Shots{ ((rateOfFire.shots * m_crew.gunCrew) % kStandardGunCrew == 0) ? (rateOfFire.shots * m_crew.gunCrew) / kStandardGunCrew  :  
		                                                                          (rateOfFire.shots * m_crew.gunCrew) / kStandardGunCrew + 1};
}

int  tankDamageSystem::TankDamageSystemStrategy::amountOfActionCanDo() const
{
	return (m_crew.overallCrewCount > 1) ? 2 :  1;
}

bool  tankDamageSystem::TankDamageSystemStrategy::isMovingSystemsAlive() const
{ 
	if (m_crew.overallCrewCount <= 0 || m_damageableParts.at(kEngineDestroyed).getState() == Damaged) 
		return false;
	//temp effects check
	if (m_damageableParts.at(kDriverKilled).isMissAction() || m_damageableParts.at(kCrewShellShocked).isMissAction()) 
		return false;
	
	return true;
}

bool tankDamageSystem::TankDamageSystemStrategy::isGunsAlive() const
{
	if (m_damageableParts.at(kGunDestroyed).getState() == Damaged || m_damageableParts.at(kCrewShellShocked).isMissAction()) 
		return false;

	return true;
}

bool tankDamageSystem::TankDamageSystemStrategy::isAlive(PointOfView pointOfView) const
{
	if (pointOfView == PointOfView::Enemy) return m_isAliveForEnemy;

	return m_isAlive;
}

void tankDamageSystem::TankDamageSystemStrategy::setDamageState(const DamageType damageType, const UnitPart::State state)
{
	if (!m_damageableParts.contains(damageType)) return;

	m_damageableParts.at(damageType).setState(state);
}

void tankDamageSystem::TankDamageSystemStrategy::nextTurn()
{
	for (auto& [name, part] : m_damageableParts) 
		part.resetMissActionOnNextTurn();
}

core::Crew core::UnitPart::getAndResetCrewEffect() const
{
	Crew tmp = m_crewEffectAfterDamage;
	if (m_state == State::Damaged) m_crewEffectAfterDamage = Crew{ 0, 0 };
	
	return tmp;
}

void core::UnitPart::setActionMissAccordingToStatus(ActionStatus actionStatus)
{
	if (m_actionEffectOnDamage != ActionEffectAfterDamage::MissAction) return;

	m_missAction = static_cast<bool>(actionStatus);
	m_missActionOnNextTurn = !static_cast<bool>(actionStatus);
}

void core::UnitPart::resetMissActionOnNextTurn()
{
	m_missAction = false;
	m_missAction = m_missActionOnNextTurn ^ false; // bitwise XOR to avoid extra branching (false ^ false = false)
	m_missActionOnNextTurn = false;
}
