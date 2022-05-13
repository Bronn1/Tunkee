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

struct ChanceToHit {
    int current;

};

using UnitTextInfo = std::vector<std::string>;
struct UnitStateMsg {
    CrewInfo crewInfo{};
    UnitPartsInfoVec unitParts{};
    /**
     * @brief Converted to text information about unit.
     * Format: first element -  unit movement info, second - rate of fire info, third - hidden damage counter, fourth -  move status;
    */
    UnitTextInfo unitInfo{4,""};
    LastActions lastActions{};
    //UnitType m_unitType;
    //ActionState m_actionState;
    UnitIdentifier id{ UnitIdentifier{0} };
    //ThreatLevel threatLvl{}
    PointOfView pointOfView{ PointOfView::Enemy };
    int hiddenDamageCounter{ 0 }; //prob can remove
    bool isAlive{ true };
    bool isRotatableTo{ false };
    bool isInLineOfSight{ false };
};

class GameMessageBuilder {
public:
    static GameMessageBuilder createUnitStateMsg(const Unit* targetUnit, const PointOfView pointOfView);
    inline UnitStateMsg getStateMsg() const { return m_stateMsg; }
    GameMessageBuilder& addMoveStatus(const LastActions lastActions);
    GameMessageBuilder& addEnemyInfo(const GameBoard& board, const Unit* selectedUnit, const DamageCalculator& calculator, const Angle& requiredGunAngleToShot);
private:
    GameMessageBuilder() = default;
private:
    UnitStateMsg m_stateMsg{};
    const Unit* m_targetUnit{ nullptr };
};
}