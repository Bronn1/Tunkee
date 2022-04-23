#pragma once

//#include "unit.h"
//#include "damage_calculator.h"
#include "unit_damage_common.h"
#include "data_types.h"
#include "game_tile.h"

namespace core
{
    class DamageCalculator;
    class Unit;
    class GameBoard;

    struct ChanceToHit{
        int current;

    };

   struct UnitStateMsg {
        CrewInfo m_crewInfo{};
        UnitPartsInfoVec m_unitParts{};
        //UnitType m_unitType;
        std::pair<UnitActions, UnitActions> m_lastActions{ UnitActions::None, UnitActions::None };
        ActionState m_actionState;
        UnitIdentifier m_id{ UnitIdentifier{0} };
        //ThreatLevel threatLvl{}
        PointOfView m_pointOfView{ PointOfView::Enemy };
        int m_hiddenDamageCounter{ 0 };
        bool m_isAlive{ true };
        bool m_isRotatableTo{ false };
        bool m_isInLineOfSight{  false };
    };

    class GameMessageFactory {
    public:
        static UnitStateMsg createUnitStateMsg(const GameBoard& board, const Unit* selectedUnit, const Unit* targetUnit, const DamageCalculator& calculator, const Angle& requiredGunAngleToShot, PointOfView pointOfView);
    };
}