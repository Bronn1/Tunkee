#pragma once

#include <string_view>
#include <array>
#include <algorithm>
#include <cassert>

constexpr std::array<std::string_view, 13> kTankDamageType = {"Burning", "Exploded", 
                                                              "CommanderKilled", "DriverKilled",
                                                              "ChargerKilled", "Transmission",
                                                              "EngineDestroyed", "TurretJammed",
                                                              "GunDestroyed", "ScopeDamaged",
                                                              "CrewKilled", "TrackDamaged", "CrewShellShocked"};

struct UnitDamageType
{
    UnitDamageType(std::string_view type)
    {
        assert(std::find(begin(kTankDamageType), end(kTankDamageType), type) != end(kTankDamageType));
        m_type = type;
    }
    bool m_isTaken{ false };

    operator bool() { return m_isTaken; }
    std::string_view m_type;
    //isRecoveryable
};