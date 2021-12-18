#include "player.h"

using namespace core;


/*
void core::Player::moveUnitTo(const UnitIdentifier& id, const GameTile& destination)
{
}

GameTile Player::getUnitPosition(const UnitIdentifier& id) const
{
    return GameTile();
}

inline bool core::Player::hasUnitWithId(const UnitIdentifier& id) const
{
    bool hasUnit = m_ownedUnits.contains(id);

    if (hasUnit)
        return hasUnit;
    else
    {
        std::cerr << "Unit with id: " << id.identifier << " does not exists\n"; // // TODO notify observer about existing units to sync up (not implemented yet)
        return false;
    }
}

bool Player::checkUnitPositionAndSetIfNot(const UnitIdentifier& id, const GameTile& viewPosition)
{
    auto [ID] = id;
    const GameTile& realPosition = m_ownedUnits.at(id).getPosition();
    if (this->hasUnitWithId(id))
        if (realPosition == viewPosition)
            return true;
        else {
            std::cerr << "Unit id: " << ID << " at position:" << m_ownedUnits.at(id).getPosition() << ". Incorrect view position is recieved. Changing to correct position..\n";
            moveUnitTo(id, realPosition); // TODO better just notify observer about model position (not implemented yet)
            return false;
        }
}*/

core::Player::Player(const PlayerIdentifier id, UnitsPickType pickType) :
    m_id(id), m_pickType(pickType)
{
}

bool core::Player::isAbleToAddUnit()
{
    return true;
}

bool core::Player::endSetupStage()
{
    m_isFinishedPickingStage = true;
    return true;
}
