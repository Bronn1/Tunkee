#pragma once

#include "data_types.h"
#include "game_tile.h"
#include "unit_damage_system_strategy.h"

#include <stack>
#include <memory>

     
enum class ActionTypes
{
    moveUnit = 1,
    shootUnit

};

enum class QueryTypes
{
    getMoveArea = 1,
    selectUnit = 2
};

/**Player requests that can not be canceled. Mostly information requests
* like get move area, get unit info and etc
*/
struct  GameQuery
{
    GameQuery(QueryTypes type, std::string_view event_name, PlayerIdentifier playerId, const  UnitIdentifier unitId) : 
        m_type(type), m_name(event_name), m_playerID(playerId), m_unitID(unitId) {};
    UnitIdentifier m_unitID{0};
    PlayerIdentifier m_playerID{0};
    const std::string m_name;
    inline QueryTypes getType() const { return m_type; }
private:
    QueryTypes m_type;
};


/** Holds information about performed action by player.
* Can be canceled by default.
*/
struct  GameAction
{
    GameAction(ActionTypes type, std::string_view event_name) : m_type(type), m_name(event_name) {};
    GameAction(PlayerIdentifier playerId, UnitIdentifier srcUnit, ActionTypes type, std::string_view event_name) 
               : m_playerID(playerId), m_type(type), m_name(event_name), m_unitID(srcUnit) {};
    UnitIdentifier m_unitID{0};
    PlayerIdentifier m_playerID{0};
    const std::string m_name;
    inline ActionTypes getType() const { return m_type; }
private:
    ActionTypes m_type;
};


struct GetMoveAreaQuery : GameQuery
{
    GetMoveAreaQuery(PlayerIdentifier playerId, UnitIdentifier unitId) : GameQuery(QueryTypes::getMoveArea, "getMoveArea", playerId, unitId) {}
};

struct MoveToAction : GameAction
{
    MoveToAction() : GameAction(ActionTypes::moveUnit, "moveTo") {}
    core::GameTile m_destination;
};

struct ShootAction : GameAction
{
    ShootAction(PlayerIdentifier playerId, const UnitIdentifier srcUnit, const UnitIdentifier target) : GameAction(playerId, srcUnit, ActionTypes::moveUnit, "shootTo"), m_target(target) {}
    UnitIdentifier  m_target;
    // do we need this here, most likely not
    std::string_view  m_damageDone;
};

struct RotateUnitActiom : GameAction
{
    enum class Type
    {
        Body,
        Gun
    };
    RotateUnitActiom(const Angle& angle, Type type) : GameAction(ActionTypes::moveUnit, "Rotation"), m_angle(angle), m_type(type) {}
    Angle m_angle;
    Type m_type;
};

struct SwitchPlayerAction
{
    PlayerIdentifier m_playerID{ 0 };
};

struct SelectUnitQuery : GameQuery
{
    SelectUnitQuery(PlayerIdentifier playerId, const  UnitIdentifier unitId) 
        : GameQuery(QueryTypes::selectUnit, "selectUnit", playerId, unitId)  {}
};

struct FinishSetupStage
{
    PlayerIdentifier m_playerID{ 0 };
};

struct FinishActionPhase
{
    PlayerIdentifier m_playerID{ 0 };
};

struct UnitSelectedInfo {
    UnitSelectedInfo(UnitIdentifier id) : m_unitId(id) {}
    UnitIdentifier m_unitId{};
};

struct UnitStateQuery {
    UnitStateQuery(UnitIdentifier selectedId, UnitIdentifier targetId, Angle requiredGunAngle, PlayerIdentifier playerId ) :
        m_selectedUnit(selectedId), m_targetUnit(targetId), m_requiredGunAngleToShot(requiredGunAngle), m_player(playerId) {}
    UnitIdentifier m_selectedUnit{};
    UnitIdentifier m_targetUnit{};
    Angle m_requiredGunAngleToShot;
    PlayerIdentifier m_player;
};

// structs like *Info containts information about changes performed in model
// model should notify view with these structures

struct MoveAreaInfo {
    MoveAreaInfo() = default;
    MoveAreaInfo(std::vector<core::GameTile> area, int half) : moveArea(area), firstLayerSize(half) {}
    //MoveAreaInfo(MoveAreaInfo&&) = default;
    MoveAreaInfo& operator=(const MoveAreaInfo& other) = default;
    std::vector<core::GameTile> moveArea{};
    int firstLayerSize{ 0 };
    UnitIdentifier m_unitId{};
};

struct MoveUnitInfo {
    MoveUnitInfo() = default;
    MoveUnitInfo(std::vector<core::GameTile>& move, core::GameTile& unitPos, UnitIdentifier unitId, HexVertexNumber rotation) :
        m_movePath(move), m_unitPos(unitPos), m_unitId(unitId), m_rotation(rotation) {}
    std::vector<core::GameTile> m_movePath{};
    HexVertexNumber m_rotation;
    UnitIdentifier m_unitId{};
    core::GameTile m_unitPos;
};

struct RotateGunInfo {
    RotateGunInfo(Angle gunRotation, UnitIdentifier id) : m_gunRotation(gunRotation), m_unitId(id) {}
    Angle m_gunRotation;
    UnitIdentifier m_unitId{};
};

struct UnitStateInfo {
    UnitStateInfo( UnitIdentifier srcUnit, UnitIdentifier targetUnit, core::DamageStatus Status) :
        m_srcUnit(srcUnit), m_targetUnit(targetUnit), m_damageStatus(Status) {}
    UnitIdentifier m_srcUnit{};
    UnitIdentifier m_targetUnit{};
    std::string_view m_damageTypeName;
    core::DamageStatus m_damageStatus;
};



