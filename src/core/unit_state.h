#pragma once

#include "data_types.h"

#include <string_view>
#include <array>
//#include <algorithm>
#include <functional>
#include <cassert>
#include <unordered_map>



namespace core {

    using PeopleCount = int;
    using DamageType = std::string_view;
    struct Crew
    {
        //Crew(PeopleCount overallNumber, PeopleCount gunNumber) : overallCrewCount(overallNumber), gunCrew(gunNumber) {}
        void set(const PeopleCount overallNumber, const  PeopleCount gunNumber) { overallCrewCount = overallNumber, gunCrew = gunNumber; }
        PeopleCount overallCrewCount;
        PeopleCount gunCrew;
    };

    struct UnitPart
    {
        enum class State
        {
            Damaged,
            Normal,
            Ideal
        };

        bool m_isVisible{ false };
        bool m_missingNextTurn{ false };
        State m_state{ State::Normal };
    };

    /**
     * @brief Class represents interface to get internal unit state . Unit parts in this class or derived classes can
     * be damaged or have some special states which may affect gaming process(moving, shooting, action states, etc). 
     * Because different type of units can differ sevirely and damage system is a big part of the game so any type of 
     * unit can have this interface as a base class and implemet his own parts, states, crew number and etc.
    */
    class UnitState
    {
    public:
        void applyDamage(const std::string_view damageType);
        virtual TileDistance getMoveDistanceWithFine(const TileDistance movement) const = 0;
        virtual Shots  getRateOfFireWithFine(const Shots rateOfFire) const = 0;
        virtual int    amountOfActionCanDo() const = 0;
        virtual bool   canMove() const = 0;
        virtual bool   canShot() const = 0;
        void setCrew(const Crew& crew) { m_crew = crew; }
        virtual ~UnitState() = default;
        UnitPart::State getScopeState() const { return  m_scope.m_state; }
        void setIdealScope() { m_scope.m_state = UnitPart::State::Ideal; }
    protected:
        Crew m_crew{0, 0};
        std::unordered_map< DamageType, std::function<void()>> m_applyDamage{};
        UnitPart m_scope;
    };
}

namespace tank_state_system 
{
    using namespace core;
    constexpr std::string_view kBurning = "Burning";
    constexpr std::string_view kExploded = "Exploded";
    constexpr std::string_view kCommanderKilled = "CommanderKilled";
    constexpr std::string_view kDriverKilled = "DriverKilled";
    constexpr std::string_view kRadiomanKilled = "RadiomanKilled";//radioman
    constexpr std::string_view kGunnerKilled = "GunnerKilled";
    constexpr std::string_view kLoaderKilled = "LoaderKilled";
    constexpr std::string_view kTransmissionDestroyed = "TransmissionDestroyed";
    constexpr std::string_view kEngineDestroyed = "EngineDestroyed";
    constexpr std::string_view kTurretJammed = "TurretJammed";
    constexpr std::string_view kGunDestroyed = "GunDestroyed";
    constexpr std::string_view kScopeDamaged = "ScopeDamaged";
    constexpr std::string_view kCrewKilled = "CrewKilled";
    constexpr std::string_view kTrackDamaged = "TrackDamaged";
    constexpr std::string_view kCrewShellShocked = "CrewShellShocked";
    constexpr std::string_view kRicochet = "Ricochet";
    constexpr std::string_view kMissed = "Missed";

    constexpr std::array<std::string_view, 16> kTankDamageTypes = {kBurning, kExploded,
                                                                   kCommanderKilled, kDriverKilled,
                                                                   kRadiomanKilled, kGunnerKilled, kLoaderKilled, kTransmissionDestroyed,
                                                                   kEngineDestroyed, kTurretJammed,
                                                                   kGunDestroyed, kScopeDamaged ,
                                                                   kCrewKilled, kTrackDamaged, kCrewShellShocked, kRicochet };

    class TankState : public core::UnitState
    {
    public:
        using enum core::UnitPart::State;
        TankState(const core::Crew& crew);
        TileDistance getMoveDistanceWithFine(const TileDistance movement) const override;
        Shots  getRateOfFireWithFine(const Shots rateOfFire) const override;
        int    amountOfActionCanDo() const override;
        bool   canMove() const override;
        bool   canShot() const override;
    protected:
        UnitPart m_burning;
        bool m_isExploded{ false };
        UnitPart m_driver;
        UnitPart m_commander;
        UnitPart m_radioman;
        UnitPart m_gunner;
        UnitPart m_loader;
        UnitPart m_transmission;
        UnitPart m_engine;
        UnitPart m_turret;
        UnitPart m_gun;
        UnitPart m_track;
        UnitPart m_isCrewShellShocked;
    };
}


