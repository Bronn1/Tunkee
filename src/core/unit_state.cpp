#include "unit_state.h"

#include <cmath>

void core::UnitState::applyDamage(const std::string_view damageType)
{
	if (!m_applyDamage.contains(damageType))
		std::cout << "Unexpected damage type is recieved:" << damageType << "\n"; // TODO throw custom exception
	   
	m_applyDamage[damageType]();
	
}


// "Burning"  - конец хода - вероятность случ события взорвался, потушен, продолжает гореть.  (дополнительное действие тушить огонь?). Ограничений нет ВИДЕН
// "Exploded" - уничтожен видно врагу ВИДЕН 
// "CommanderKilled" - ? (уменьшается скорость стрельбы) 
// "DriverKilled" -  ? в этот ход ехать не может,может ли кто то занять его место? соотвественно -1 человек в башне
// "loaderKilled" -  ? (когда остался один член команды, то может либо стрелять, либо ездить. 
// Если людей меньше в башне, то штрафы к скоростельности, (loader, gunner, Driver)               round up ((rate of fire* человек в башне) / 3) 
// "gunnerKilled" - ?   
// запасной член экипажа в некоторых танках?
// "TransmissionDestroyed"  - movement = 1;
// "EngineDestroyed" cant move immobile
// "TurretJammed"  - turret can't rotate
// "GunDestroyed" - can't fire (invisible),ai тоже видеть не должен. С неполным туманом войны, можно показывать, где то конфигурить? .
// "ScopeDamaged" - штраф к вероятности попадания( +1 или +2). бонус хорошая оптика -1 к попаданию 
// "CrewKilled" - танк уничтожен, но эффект невиден
// "TrackDamaged" - первый трек только крутиться, обездвижен. ВИДЕН
// "CrewShellShocked" - пропуск одного хода,  эффект невиден. 
// задавать сколько человек в башне и общее кол-во
//  range - 20
// отображать в гуе не словами, а карточками damage

tank_state_system::TankState::TankState(const Crew& crew)
{
	setCrew(crew);
	// TODO maybe add all this to just map ??????
	m_applyDamage[kBurning] = [parts = &(*this)]() { parts->m_burning.m_state = Damaged; };
	m_applyDamage[kExploded] = [parts = &(*this)]() { parts->m_isExploded = true; };
	m_applyDamage[kCommanderKilled] = [parts = &(*this)]() {
		if (parts->m_commander.m_state != Damaged)
		{
			parts->m_commander.m_state = Damaged;
			parts->m_crew.overallCrewCount -= 1;
		}
	};
	m_applyDamage[kDriverKilled] = [parts = &(*this)]() {
		if (parts->m_driver.m_state != Damaged)
		{
			parts->m_driver.m_state = Damaged;
			parts->m_crew.gunCrew -= 1;
			parts->m_crew.overallCrewCount -= 1;
		}
	};
	m_applyDamage[kChargerKilled] = [parts = &(*this)]() {
		if (parts->m_charger.m_state != Damaged)
		{
			parts->m_charger.m_state = Damaged;
			parts->m_crew.overallCrewCount -= 1;
		}
	};
	m_applyDamage[kGunnerKilled] = [parts = &(*this)]() {
		if (parts->m_gunner.m_state != Damaged)
		{
			parts->m_gunner.m_state = Damaged;
			parts->m_crew.gunCrew -= 1;
			parts->m_crew.overallCrewCount -= 1;
		}
	};
	m_applyDamage[kLoaderKilled] = [parts = &(*this)]() {
		if (parts->m_loader.m_state != Damaged)
		{
			parts->m_loader.m_state = Damaged;
			parts->m_crew.gunCrew -= 1;
			parts->m_crew.overallCrewCount -= 1;
		}
	};
	m_applyDamage[kTransmissionDestroyed] = [parts = &(*this)]() { parts->m_transmission.m_state = Damaged; };
	m_applyDamage[kEngineDestroyed] = [parts = &(*this)]() { parts->m_engine.m_state = Damaged; };
	m_applyDamage[kTurretJammed] = [parts = &(*this)]() { parts->m_turret.m_state = Damaged; };
	m_applyDamage[kGunDestroyed] = [parts = &(*this)]() { parts->m_gun.m_state = Damaged; };
	m_applyDamage[kScopeDamaged] = [parts = &(*this)]() { parts->m_scope.m_state = Damaged; };
	m_applyDamage[kCrewKilled] = [parts = &(*this)]() { parts->m_crew = Crew{ 0,0 }; };
	m_applyDamage[kTrackDamaged] = [parts = &(*this)]() { parts->m_track.m_state = Damaged; };
	m_applyDamage[kCrewShellShocked] = [parts = &(*this)]() { parts->m_isCrewShellShocked.m_state = Damaged; };
	m_applyDamage[kRicochet] = []() {};
}

TileDistance  tank_state_system::TankState::getMoveDistanceWithFine(const TileDistance movement) const
{
	if (m_crew.overallCrewCount <= 0 || m_engine.m_state == Damaged) return TileDistance{ 0 };

	if (m_transmission.m_state == Damaged) return TileDistance{ 1 };

	return movement;
}

Shots  tank_state_system::TankState::getRateOfFireWithFine(const Shots rateOfFire) const
{
	
	if (m_crew.overallCrewCount <= 0 || m_gun.m_state == Damaged) return Shots{0};

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
	if (m_crew.overallCrewCount <= 0 || m_engine.m_state == Damaged) return false;
	
	return true;
}

bool tank_state_system::TankState::canShot() const
{
	if (m_gun.m_state == Damaged) return false;

	return true;
}
