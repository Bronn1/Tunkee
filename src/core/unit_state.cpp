#include "unit_state.h"

#include <cmath>

void tank_state_system::TankState::applyDamage(const std::string_view damageType)
{
	if (!m_damageableParts.contains(damageType))
	{
		std::cout << "Unexpected damage type is recieved:" << damageType << "\n"; // TODO throw custom exception
		return;
	}

	m_damageableParts.at(damageType).applyDamage();
	m_crew -= m_damageableParts.at(damageType).getCrewEffect();
	
	if (!m_crew) m_isAlive = false;
}

// ������� ������ �����-> ������� ������ ���� -> ����������� ���� �������� ����, ����� ����, ��������� ���� � ������������ ���� ����� ���� ������ � ���������� �����
// ������������ �������� ������������� ������ �� ����� ������� �� ��������� ������ 60 ��������, ������ ���� ��������
// ���� ������� �� ������, ������ ���� ���� �������� ��� ������ ������� ���� ����
// ����� offset � ������, ������� ����� ������� 
// ����� ��������� ����� � ������ �������, �� ���� ��� �� ��� � �������� �� ���� � ������ 
// ���� ��� ��������, �� ����� ��� ���� ������ �����
// 
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

tank_state_system::TankState::TankState(const Crew& crew)
{
	setCrew(crew);
	m_damageableParts[kBurning] = UnitPart();
	m_damageableParts[kExploded] = UnitPart();
	m_damageableParts[kCommanderKilled] = UnitPart(Crew(1, 0));// Crew in constructur is how damage to this part effects crew count
	m_damageableParts[kDriverKilled] = UnitPart(Crew(1, 1));
	m_damageableParts[kRadiomanKilled] = UnitPart(Crew(1, 0));
	m_damageableParts[kGunnerKilled] = UnitPart(Crew(1, 1));
	m_damageableParts[kLoaderKilled] = UnitPart(Crew(1, 1));
	m_damageableParts[kTransmissionDestroyed] = UnitPart();
	m_damageableParts[kEngineDestroyed] = UnitPart();
	m_damageableParts[kTurretJammed] = UnitPart();
	m_damageableParts[kGunDestroyed] = UnitPart();
	m_damageableParts[kScopeDamaged] = UnitPart();
	m_damageableParts[kTrackDamaged] = UnitPart();
	m_damageableParts[kCrewShellShocked] = UnitPart();
	m_damageableParts[kCrewKilled] = UnitPart(Crew(999, 999));
}

TileDistance  tank_state_system::TankState::getMoveDistanceWithFine(const TileDistance movement) const
{
	//maybe better to add vars to unit parts can move after damage and can shoot after damage , it will reduce amount of if's in code
	if (m_crew.overallCrewCount <= 0 || m_damageableParts.at(kEngineDestroyed).getState()== Damaged) return TileDistance{ 0 };

	if (m_damageableParts.at(kTransmissionDestroyed).getState() == Damaged) return TileDistance{ 1 };

	return movement;
}

Shots  tank_state_system::TankState::getRateOfFireWithFine(const Shots rateOfFire) const
{
	if (m_crew.overallCrewCount <= 0 || m_damageableParts.at(kGunDestroyed).getState() == Damaged) return Shots{0};

	if (m_crew.gunCrew <= 0 || m_crew.overallCrewCount == 1)
		return Shots{ 1 };

	const unsigned kStandardGunCrew = 3;
	// cant use std::ceil with unsigned 
	return Shots{ ((rateOfFire.shots * m_crew.gunCrew) % kStandardGunCrew == 0) ? (rateOfFire.shots * m_crew.gunCrew) / kStandardGunCrew  :  
		                                                                          (rateOfFire.shots * m_crew.gunCrew) / kStandardGunCrew + 1};
}

int  tank_state_system::TankState::amountOfActionCanDo() const
{
	return 0;
}

bool  tank_state_system::TankState::canMove() const
{
	if (m_crew.overallCrewCount <= 0 || m_damageableParts.at(kEngineDestroyed).getState() == Damaged) return false;
	
	return true;
}

bool tank_state_system::TankState::canShot() const
{
	if (m_damageableParts.at(kGunDestroyed).getState() == Damaged ) return false;

	return true;
}


core::Crew core::UnitPart::getCrewEffect() const
{
	Crew tmp = m_crewEffectAfterDamage;
	if (m_state == State::Damaged) m_crewEffectAfterDamage = Crew{ 0, 0 };
	
	return tmp;
}
