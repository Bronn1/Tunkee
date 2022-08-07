#pragma once

#include "data_types.h"
#include "game_command.h"
#include "player.h"

#include <vector>

namespace core
{
class Unit;
/**
 * @brief Interface for game main engine of the game.
 * Additional level of abstraction is needed because in case of multiplayer game we'll have 2 engines. One  type is  host engine with all game internal state.
 * And second is network engine which  connects to host, make requests and maybe caches some state before give it to view 
 * 
*/
class IEngine
{
public:
    virtual void moveUnit(MoveToAction* action) = 0;
    virtual void shootUnit(ShootAction* action) = 0;
    virtual void requestMoveArea(GetMoveAreaQuery* moveAreaQuery) = 0;
    virtual  void selectUnit(const SelectUnitQuery* selectUnitQuery) = 0;
    virtual void createUnitStateMsg(const UnitStateQuery* unitStateQuery) = 0;
    virtual UnitIdentifier addNewUnit(std::unique_ptr<Unit> unit) = 0;
    virtual void rotateUnitGun(RotateUnitActiom* rotateAction) = 0;

    virtual void finishSetupStage(FinishSetupStage* finishSetupStageAction) = 0;
    virtual void finishActionPhase(FinishActionPhase* finishActionPhase) = 0;
    virtual std::vector<UnitIdentifier> getUnitIDsForPlayer(const PlayerIdentifier playerId) const = 0;
    //virtual void undoLastCommand(const ActionPtr& cmd) = 0;
    virtual void endOfTurn() = 0;
    virtual void setPlayer(const Player& player) = 0;
    virtual ~IEngine() = default;
private:
};

} // namespace core
