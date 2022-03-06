#pragma once

#include "data_types.h"
#include "unit_damage_common.h"

#include <string_view>
#include <array>
//#include <algorithm>
#include <functional>
#include <cassert>
#include <unordered_map>



namespace core 
{
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
        virtual void   setDamageVisibleFor(const std::vector<DamageTo>& damageNames = {}) = 0;
        virtual CrewInfo   getCrewInfo() const = 0;
        virtual ~IDamageSystemStrategy() = default;
        virtual DamageStatus getDamageStatus(const DamageTo damageOf ) const = 0;
        virtual UnitPartsInfoVec getPartsInfo() const = 0;
        virtual void         setDamageStatus(const DamageTo, const  DamageStatus  state) = 0;
        //void setCrew(const CrewAmount& crew) { m_crew = crew; }
    protected:
        Crew m_crew{};

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
    constexpr std::string_view kCommander = "Commander";
    constexpr std::string_view kDriver = "Driver";
    constexpr std::string_view kRadioman = "Radioman";
    constexpr std::string_view kGunner = "Gunner";
    constexpr std::string_view kLoader = "Loader";
    constexpr std::string_view kTransmission = "Transmission";
    constexpr std::string_view kEngine = "Engine";
    constexpr std::string_view kTurretJammed = "Turret Jammed";
    constexpr std::string_view kGunDestroyed = "Gun";
    constexpr std::string_view kScope = "Scope";
    constexpr std::string_view kCrewKilled = "CrewKilled";
    constexpr std::string_view kTrack = "Track";
    constexpr std::string_view kCrewShellShocked = "CrewShellShocked";
    constexpr std::string_view kRicochet = "Ricochet";

    enum class  ActionMiss {
        none,
        currentTurn,
        nextTurn
    };

    constexpr std::array<std::string_view, 16> kTankDamageTypes = {kBurning, kExploded, kCommander, kDriver,
                                                                   kRadioman, kGunner, kLoader, kTransmission,
                                                                   kEngine, kTurretJammed, kGunDestroyed, kScope ,
                                                                   kCrewKilled, kTrack, kCrewShellShocked, kRicochet };

    class TankDamageSystemStrategy final  : public core::IDamageSystemStrategy
    {
    public:
        using enum DamageStatus;
        void applyDamage(const std::string_view damageType, ActionStatus actionStatus) override;
        TankDamageSystemStrategy(CrewInfo info);
        TileDistance getMoveDistanceWithFine(const TileDistance movement) const override;
        Shots  getRateOfFireWithFine(const Shots rateOfFire) const override;
        int    amountOfActionCanDo() const override;
        bool   isMovingSystemsAlive() const override;
        bool   isGunsAlive() const override;
        bool   isGunsRotatable() const { return m_damageableParts.at(kTurretJammed).getState() != Damaged; }
        bool   isAlive(PointOfView pointOfView) const override; 
        DamageStatus getDamageStatus(const DamageTo damageOf) const override { return m_damageableParts.at(damageOf).getState(); } // TODO POTENTIAL THROW REFACTOR
        void setDamageStatus(const DamageTo damageType, const  DamageStatus  state) override;
        void   nextTurn() override;
        void   setDamageVisibleFor(const std::vector<DamageTo>& damageNames = {}) override;
        CrewInfo   getCrewInfo() const override { return m_crew.getCrewInfo(); }
        UnitPartsInfoVec getPartsInfo() const;
    private:
        void setActionMiss(const std::string_view damageType, ActionStatus actionStatus);
        void resetMissActionOnNextTurn();
        void checkAliveSystemsAfterDamage();

        std::unordered_map< DamageTo, UnitPart> m_damageableParts{};
        std::unordered_map<DamageTo, ActionMiss> m_missingAction{ {kDriver, ActionMiss::none}, {kCrewShellShocked, ActionMiss::none} };
        bool m_isAlive{ true };
        /**
         * @brief With fog of war enemy cannot see some fatal damage(for example crew killed )
        */
        bool m_isAliveForEnemy{ true };
    };
}