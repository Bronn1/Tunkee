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
        enum class ActionEffectAfterDamage
        {
            None,
            MissAction
        };
        UnitPart() = default;
        UnitPart(Crew crewEffectAfterDamage, ActionEffectAfterDamage effect = ActionEffectAfterDamage::None) : m_crewEffectAfterDamage(crewEffectAfterDamage), m_actionEffectOnDamage(effect){}
        void applyDamage() { m_state = State::Damaged; }
        Crew getAndResetCrewEffect() const;
        State getState() const { return m_state; }
        void setState(State state) { m_state = state; }
        bool isVisibleForEnemy() const { return m_isVisibleForEnemy; }
        void setActionMissAccordingToStatus(ActionStatus actionStatus);
        bool isMissAction() const { return m_missAction; }
        void resetMissActionOnNextTurn();
    private:
        mutable Crew  m_crewEffectAfterDamage{ 0, 0 }; // applies only once 
        bool m_isVisibleForEnemy{ false };
        bool m_missAction{ false };
        bool m_missActionOnNextTurn{ false };
        ActionEffectAfterDamage m_actionEffectOnDamage{ ActionEffectAfterDamage::None };
        State m_state{ State::Normal };

    };

    //template<typename T>
    //concept UnitStateSystem = requires(T type)
    //{
     //   type.applyDamage(std::declval< const std::string_view>);
    //};
    //template<typename T> requires UnitStateSystem<T>

    enum  class PointOfView{
        Player,
        Enemy
    };
    enum class DamageSystemStrategies {
        TankDamageSystem
    };
    /**
     * @brief Class represents interface to get internal unit  system of damage. Unit parts in this class or derived classes can
     * be damaged or have some special states which may affect gaming process(moving, shooting, action states, etc). 
     * Because different type of units can differ sevirely and damage system is a big part of the game so any type of 
     * unit can implement his owm strategy.
    */
    class IDamageSystemStrategy
    {
    public:
        virtual void applyDamage(const std::string_view damageType, ActionStatus actionStatus) = 0;
        virtual TileDistance getMoveDistanceWithFine(const TileDistance movement) const = 0;
        virtual Shots  getRateOfFireWithFine(const Shots rateOfFire) const = 0;
        virtual int    amountOfActionCanDo() const = 0;
        virtual bool   isMovingSystemsAlive() const = 0;
        virtual bool   isGunsAlive() const = 0;
        virtual bool   isGunsRotatable() const = 0;
        virtual bool   isAlive(PointOfView pointOfView) const = 0;
        virtual void   nextTurn() = 0;
        virtual ~IDamageSystemStrategy() = default;
        virtual UnitPart::State getScopeState() const = 0;
        virtual void setDamageState(const DamageType, const  UnitPart::State  state) = 0;
        void setCrew(const Crew& crew) { m_crew = crew; }
    protected:
        Crew m_crew{0, 0};

    };
}

/**
 * @brief here will be specific constants for tank and tank damage system strategy
*/
namespace tankDamageSystem 
{
    using namespace core;
    // this names also are names in tables of damage probabilities
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

    class TankDamageSystemStrategy final  : public core::IDamageSystemStrategy
    {
    public:
        using enum core::UnitPart::State;
        void applyDamage(const std::string_view damageType, ActionStatus actionStatus) override;
        TankDamageSystemStrategy(const core::Crew& crew);
        TileDistance getMoveDistanceWithFine(const TileDistance movement) const override;
        Shots  getRateOfFireWithFine(const Shots rateOfFire) const override;
        int    amountOfActionCanDo() const override;
        bool   isMovingSystemsAlive() const override;
        bool   isGunsAlive() const override;
        bool   isGunsRotatable() const { return m_damageableParts.at(kTurretJammed).getState() != Damaged; }
        bool   isAlive(PointOfView pointOfView) const override;
        UnitPart::State getScopeState() const override { return m_damageableParts.at(kScopeDamaged).getState(); }
        void setDamageState(const DamageType damageType, const  UnitPart::State  state) override;
        void   nextTurn();
    private:
        void checkAliveSystemsAfterDamage();
        std::unordered_map< DamageType, UnitPart> m_damageableParts{};
        bool m_isAlive{ true };
        /**
         * @brief With fog of war enemy cannot see some fatal damage(for example crew killed )
        */
        bool m_isAliveForEnemy{ true };
    };
}


