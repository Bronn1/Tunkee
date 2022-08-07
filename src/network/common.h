#pragma once

#include "src/core/game_settings.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

namespace network
{

constexpr size_t kSecretTest = 123210432848324343;

struct JoinGameMsg
{
    std::string guestPlayerName{""};
    size_t secret{0};
    PlayerIdentifier guestPlayerId{ 0 };
};

struct GameInfoMsg 
{
    settings::GameSettings settings;
    size_t secret{ 0 };
};

} // namespace network

/**
 * @brief contains serialization methods
 * TODO: how to be sure that every change made to data also force to change serialization methods here(version or assertions?)
*/
namespace cereal
{

template<class Archive>
void serialize(Archive& archive,
               PlayerIdentifier& id)
{
    archive(id.identifier);
}

template<class Archive>
void serialize(Archive& archive,
               setup::Money& id)
{
    archive(id.amount);
}

template<class Archive>
void serialize(Archive& archive,
               core::GameTile& tile)
{
    archive(tile.m_q, tile.m_r, tile.m_s, tile.m_type, tile.isAccessible);
}

template<class Archive>
void serialize(Archive& archive,
               settings::MapSize mapSize)
{
    assert(sizeof(mapSize) == 8U);
    archive(mapSize.width, mapSize.height);
}



template<class Archive>
void serialize(Archive& archive,
               network::JoinGameMsg& joinMsg)
{
    archive( CEREAL_NVP(joinMsg.guestPlayerName), joinMsg.secret, joinMsg.guestPlayerId);
}

template<class Archive>
void serialize(Archive& archive,
               settings::GameSettings& setting)
{
    archive(CEREAL_NVP(setting.setupArea), setting.hostName, setting.guestName, setting.mapSize,
            setting.hostId, setting.guestId, setting.amount, setting.type, setting.boardType);
}

template<class Archive>
void serialize(Archive& archive,
               network::GameInfoMsg& infoMsg)
{
    archive( CEREAL_NVP(infoMsg.settings), infoMsg.secret );
}

} // namespace  cereal
