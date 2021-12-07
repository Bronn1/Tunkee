#pragma once

#include "data_types.h"
#include "game_tile.h"

#include <stack>
#include <memory>

     
enum class ActionTypes
{
	moveUnit = 1

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

struct SwitchPlayerAction
{
	PlayerIdentifier m_playerID{ 0 };
};

struct SelectUnitQuery : GameQuery
{
	SelectUnitQuery(PlayerIdentifier playerId, const  UnitIdentifier unitId) 
		: GameQuery(QueryTypes::selectUnit, "selectUnit", playerId, unitId)  {}
};

struct FinishSetupStageAction
{
	PlayerIdentifier m_playerID{ 0 };
};

struct UnitSelectedInfo {
	UnitSelectedInfo(UnitIdentifier id) : m_unitId(id) {}
	UnitIdentifier m_unitId{};
};

struct GameInfoMessage {
	GameInfoMessage(UnitIdentifier unitId,  PlayerIdentifier playerId, const std::string& info ) : 
		m_unitId(unitId), m_player(playerId), m_message(info) {}
	UnitIdentifier m_unitId{};
	PlayerIdentifier m_player;
	std::string m_message{}; // maybe string_view better here??
};

// structs like *Info containts information which view should know about changes in game model(core)
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
	MoveUnitInfo(std::vector<core::GameTile>& move, UnitIdentifier unitId) :
		m_movePath(move), m_unitId(unitId) {}
	std::vector<core::GameTile> m_movePath{};
	UnitIdentifier m_unitId{};
};



