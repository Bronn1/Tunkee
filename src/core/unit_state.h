#pragma once

#include "data_types.h"

#include <string_view>
#include <array>
//#include <algorithm>
#include <functional>
#include <cassert>
#include <unordered_map>



namespace core 
{
    constexpr std::string_view kShotMissed = "Missed";

    using PeopleCount = int;
    using DamageType = std::string_view;
    struct Crew
    {
        explicit operator bool() {
            return overallCrewCount > 0;
        }
        Crew& operator-=(const Crew& other)
        { 
            overallCrewCount -= other.overallCrewCount;
            gunCrew -= other.gunCrew;
            return *this;
        }
        PeopleCount overallCrewCount;
        PeopleCount gunCrew;
    };

    class UnitPart
    {
    public:
        enum class State
        {
            Damaged,
            Normal,
            Ideal
        };
        UnitPart() = default;
        UnitPart(Crew crewEffectAfterDamage) : m_crewEffectAfterDamage(crewEffectAfterDamage) {}
        void applyDamage() { m_state = State::Damaged; }
        Crew getCrewEffect() const;
        State getState() const { return m_state; }
        void setState(State state) { m_state = state; }
    private:
        mutable Crew  m_crewEffectAfterDamage{ 0, 0 };
        bool m_isVisibleForEnemy{ false };
        bool m_missingNextTurn{ false };
        State m_state{ State::Normal };

    };

    template<typename T>
    concept UnitStateSystem = requires(T type)
    {
        type.applyDamage(std::declval< const std::string_view>);
    };

    //template<typename T> requires UnitStateSystem<T>

    /**
     * @brief Class represents interface to get internal unit state . Unit parts in this class or derived classes can
     * be damaged or have some special states which may affect gaming process(moving, shooting, action states, etc). 
     * Because different type of units can differ sevirely and damage system is a big part of the game so any type of 
     * unit can have this interface as a base class and implement his own parts, states, crew number and etc.
    */
    class UnitStateInterface
    {
    public:
        virtual void applyDamage(const std::string_view damageType) = 0;
        virtual TileDistance getMoveDistanceWithFine(const TileDistance movement) const = 0;
        virtual Shots  getRateOfFireWithFine(const Shots rateOfFire) const = 0;
        virtual int    amountOfActionCanDo() const = 0;
        virtual bool   canMove() const = 0;
        virtual bool   canShot() const = 0;
        virtual bool   isAlive() const = 0;
        virtual ~UnitStateInterface() = default;
        virtual UnitPart::State getScopeState() const = 0;//{ return  m_scope.m_state; }
        virtual void setIdealScope() = 0;//{ m_scope.m_state = UnitPart::State::Ideal; }
        void setCrew(const Crew& crew) { m_crew = crew; }
    protected:
        Crew m_crew{0, 0};

    };
}

/**
 * @brief here will be specific constants for tank and tank state(or condition)
*/
namespace tank_state_system 
{
    using namespace core;
    constexpr std::string_view kBurning = "Burning";
    constexpr std::string_view kExploded = "Exploded";
    constexpr std::string_view kCommanderKilled = "CommanderKilled";
    constexpr std::string_view kDriverKilled = "DriverKilled";
    constexpr std::string_view kRadiomanKilled = "RadiomanKilled";
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

    constexpr std::array<std::string_view, 16> kTankDamageTypes = {kBurning, kExploded,
                                                                   kCommanderKilled, kDriverKilled,
                                                                   kRadiomanKilled, kGunnerKilled, kLoaderKilled, kTransmissionDestroyed,
                                                                   kEngineDestroyed, kTurretJammed,
                                                                   kGunDestroyed, kScopeDamaged ,
                                                                   kCrewKilled, kTrackDamaged, kCrewShellShocked, kRicochet };

    class TankState : public core::UnitStateInterface
    {
    public:
        using enum core::UnitPart::State;
        void applyDamage(const std::string_view damageType) override;
        TankState(const core::Crew& crew);
        TileDistance getMoveDistanceWithFine(const TileDistance movement) const override;
        Shots  getRateOfFireWithFine(const Shots rateOfFire) const override;
        int    amountOfActionCanDo() const override;
        bool   canMove() const override;
        bool   canShot() const override;
        bool   isAlive() const override { return m_isAlive; }
        UnitPart::State getScopeState() const override { return m_damageableParts.at(kScopeDamaged).getState(); }
        void setIdealScope() override { m_damageableParts.at(kScopeDamaged).setState(UnitPart::State::Ideal); }
    private:
        std::unordered_map< DamageType, UnitPart> m_damageableParts{};
        bool m_isAlive{ true };
    };
}


