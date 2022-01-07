#include "unit_damage_type.h"

#include <cmath>

void core::UnitCondition::applyDamage(const std::string_view damageType)
{
	if (!m_applyDamage.contains(damageType))
		std::cout << "Unexpected damage type is recieved:" << damageType << "\n"; // TODO throw custom exception
	   
	m_applyDamage[damageType]();
	
}


// "Burning"  - ����� ���� - ����������� ���� ������� ���������, �������, ���������� ������.  (�������������� �������� ������ �����?). ����������� ��� �����
// "Exploded" - ��������� ����� ����� ����� 
// "CommanderKilled" - ? (����������� �������� ��������) 
// "DriverKilled" -  ? � ���� ��� ����� �� �����,����� �� ��� �� ������ ��� �����? ������������� -1 ������� � �����
// "loaderKilled" -  ? (����� ������� ���� ���� �������, �� ����� ���� ��������, ���� ������. 
// ���� ����� ������ � �����, �� ������ � ���������������, (loader, gunner, Driver)               round up ((rate of fire* ������� � �����) / 3) 
// "gunnerKilled" - ?   
// �������� ���� ������� � ��������� ������?
// "TransmissionDestroyed"  - movement = 1;
// "EngineDestroyed" cant move immobile
// "TurretJammed"  - turret can't rotate
// "GunDestroyed" - can't fire (invisible),ai ���� ������ �� ������. � �������� ������� �����, ����� ����������, ��� �� �����������? .
// "ScopeDamaged" - ����� � ����������� ���������( +1 ��� +2). ����� ������� ������ -1 � ��������� 
// "CrewKilled" - ���� ���������, �� ������ �������
// "TrackDamaged" - ������ ���� ������ ���������, ����������. �����
// "CrewShellShocked" - ������� ������ ����,  ������ �������. 
// �������� ������� ������� � ����� � ����� ���-��
//  range - 20
// ���������� � ��� �� �������, � ���������� damage

tank_damage_system::TankCondition::TankCondition(const Crew& crew)
{
	setCrew(crew);
	// TODO maybe add all this to just map ??????
	m_applyDamage[kBurning] = [parts = &(*this)]() { parts->m_burning.m_condition = Damaged; };
	m_applyDamage[kExploded] = [parts = &(*this)]() { parts->m_isExploded = true; };
	m_applyDamage[kCommanderKilled] = [parts = &(*this)]() {
		if (parts->m_commander.m_condition != Damaged)
		{
			parts->m_commander.m_condition = Damaged;
			parts->m_crew.overallCrewCount -= 1;
		}
	};
	m_applyDamage[kDriverKilled] = [parts = &(*this)]() {
		if (parts->m_driver.m_condition != Damaged)
		{
			parts->m_driver.m_condition = Damaged;
			parts->m_crew.gunCrew -= 1;
			parts->m_crew.overallCrewCount -= 1;
		}
	};
	m_applyDamage[kChargerKilled] = [parts = &(*this)]() {
		if (parts->m_charger.m_condition != Damaged)
		{
			parts->m_charger.m_condition = Damaged;
			parts->m_crew.overallCrewCount -= 1;
		}
	};
	m_applyDamage[kGunnerKilled] = [parts = &(*this)]() {
		if (parts->m_gunner.m_condition != Damaged)
		{
			parts->m_gunner.m_condition = Damaged;
			parts->m_crew.gunCrew -= 1;
			parts->m_crew.overallCrewCount -= 1;
		}
	};
	m_applyDamage[kLoaderKilled] = [parts = &(*this)]() {
		if (parts->m_loader.m_condition != Damaged)
		{
			parts->m_loader.m_condition = Damaged;
			parts->m_crew.gunCrew -= 1;
			parts->m_crew.overallCrewCount -= 1;
		}
	};
	m_applyDamage[kTransmissionDestroyed] = [parts = &(*this)]() { parts->m_transmission.m_condition = Damaged; };
	m_applyDamage[kEngineDestroyed] = [parts = &(*this)]() { parts->m_engine.m_condition = Damaged; };
	m_applyDamage[kTurretJammed] = [parts = &(*this)]() { parts->m_turret.m_condition = Damaged; };
	m_applyDamage[kGunDestroyed] = [parts = &(*this)]() { parts->m_gun.m_condition = Damaged; };
	m_applyDamage[kScopeDamaged] = [parts = &(*this)]() { parts->m_scope.m_condition = Damaged; };
	m_applyDamage[kCrewKilled] = [parts = &(*this)]() { parts->m_crew = Crew{ 0,0 }; };
	m_applyDamage[kTrackDamaged] = [parts = &(*this)]() { parts->m_track.m_condition = Damaged; };
	m_applyDamage[kCrewShellShocked] = [parts = &(*this)]() { parts->m_isCrewShellShocked.m_condition = Damaged; };
}

TileDistance  tank_damage_system::TankCondition::getMoveDistanceWithFine(const TileDistance movement) const
{
	if (m_crew.overallCrewCount <= 0 || m_engine.m_condition == Damaged) return TileDistance{ 0 };

	if (m_transmission.m_condition == Damaged) return TileDistance{ 1 };

	return movement;
}

Shots  tank_damage_system::TankCondition::getRateOfFireWithFine(const Shots rateOfFire) const
{
	
	if (m_crew.overallCrewCount <= 0) return Shots{0};


	if (m_crew.gunCrew <= 0 || m_crew.overallCrewCount == 1)
		return Shots{ 1 };

	const unsigned kStandardGunCrew = 3;
	// cant use std::ceil with unsigned 
	return Shots{ ((rateOfFire.shots * m_crew.gunCrew) % kStandardGunCrew == 0) ? (rateOfFire.shots * m_crew.gunCrew) / kStandardGunCrew  :  
		                                                                          (rateOfFire.shots * m_crew.gunCrew) / kStandardGunCrew + 1};
}

int  tank_damage_system::TankCondition::amountOfActionCanDo() const
{
	return 0;
}

bool  tank_damage_system::TankCondition::canMove() const
{
	if (m_crew.overallCrewCount <= 0 || m_engine.m_condition == Damaged) return false;
	
	return true;
}

bool tank_damage_system::TankCondition::canShot() const
{
	if (m_gun.m_condition == Damaged) return false;

	return true;
}
