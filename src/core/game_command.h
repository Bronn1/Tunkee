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
    getMoveArea = 1
};

/**Player requests that can not be canceled. Mostly information requests
* like get move area, get unit info and etc
*/
struct  GameQuery
{
    GameQuery(QueryTypes type, std::string_view event_name) : m_type(type), m_name(event_name) {};
    ObjectIdentifier m_unitID{0};
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
	ObjectIdentifier m_unitID{0};
	PlayerIdentifier m_playerID{0};
	const std::string m_name;
	inline ActionTypes getType() const { return m_type; }
private:
	ActionTypes m_type;
};


struct GetMoveAreaQuery : GameQuery
{
	GetMoveAreaQuery() : GameQuery(QueryTypes::getMoveArea, "getMoveArea") {}
};

struct MoveToAction : GameAction
{
	MoveToAction() : GameAction(ActionTypes::moveUnit, "moveTo") {}
	core::GameTile m_destination;
};


