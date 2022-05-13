#include "player.h"


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
