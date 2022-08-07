#pragma once

#include "src/core/data_types.h"
#include "src/core/game_settings.h"
#include "common.h"

#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/awaitable.hpp>
#include <filesystem>

namespace network
{
using namespace std::literals;

class Connection
{
public:
    Connection(asio::ip::tcp::socket&& client) :
        m_client(std::move(client))
    {

    }
private:
    asio::ip::tcp::socket m_client;
    asio::ip::tcp::socket m_server{ m_client.get_executor() };

};

// TODO rename prob
class NetworkController
{
public:
    NetworkController(settings::GameSettings& gameSettings, const short port);
    std::size_t run()
    {
        return m_contex.run();
    }

    std::size_t poll()
    {
        return m_contex.poll();
    }

    std::size_t readSecret(const std::filesystem::path&) const;

    
private:
   asio::awaitable<void> listen();
   bool isNewConnectionValid(const JoinGameMsg& joinmsg);
   void updateGameSettings(const  JoinGameMsg& joinMsg);

private:
    // dont like ref as attributes, will remove after, when i'll have list of all requirements needed for this object
    settings::GameSettings& m_gameSettings;

    asio::io_context m_contex;
    asio::ip::tcp::acceptor m_acceptor;
};
} // namespace network
