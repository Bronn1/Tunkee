#include "network_controller.h"


#include <asio/ip/tcp.hpp>

#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/write.hpp>
#include <asio/streambuf.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

using std::filesystem::current_path;

namespace network
{

constexpr std::string_view kListenHostIp = "127.0.0.1";

NetworkController::NetworkController(settings::GameSettings& gameSettings, const short port) : 
    m_gameSettings(gameSettings), m_acceptor(m_contex, {asio::ip::tcp::v4(), static_cast<asio::ip::port_type>(port)})

{
    readSecret("secret.sec");
    asio::co_spawn(m_contex, this->listen(), asio::detached);
}

std::size_t NetworkController::readSecret(const std::filesystem::path& path) const
{
    //std::fstream secretFile(path, std::ios::binary | std::ios::out);
    //std::cout << current_path() << '\n';
    //if(!secretFile.is_open()){
    //    throw std::runtime_error("Cannot open secret file for network connections" ); // TODO custom exception
    //}
    return kSecretTest;
}

asio::awaitable<void> NetworkController::listen()
{
    // TODO cereal is easy to use, but every time u need to add some stream which is a bit slow
    // i think it won't be an issue here but better bear that in mind
    const size_t kBytes = 512u;
    auto remotePlayer = co_await m_acceptor.async_accept(asio::use_awaitable);
    std::string rawData(kBytes, '\0');
    JoinGameMsg joinGameMsg;
    try {
        std::size_t bytes_read = co_await remotePlayer.async_read_some(asio::buffer(rawData), asio::use_awaitable);
        rawData.resize(bytes_read);
        std::stringstream serializationStream(std::move(rawData), std::ios::binary | std::ios::out | std::ios::in);
        cereal::BinaryInputArchive serializeTo(serializationStream);
        serializeTo(joinGameMsg);
        if (!isNewConnectionValid(joinGameMsg)) {
            listen();
        }
        updateGameSettings(joinGameMsg);

        GameInfoMsg infoMsg{ m_gameSettings, kSecretTest }; 
        serializationStream.clear();
        cereal::BinaryOutputArchive serializeToBinary(serializationStream);
        serializeToBinary(infoMsg);
        // TODO check for network errors and bytes sent 0, prob this errors must be caught in calling code
        size_t bytes_sent = co_await  asio::async_write(remotePlayer, asio::buffer(serializationStream.str()), asio::use_awaitable);
        //m_is_connected = true
    }
    catch (const cereal::Exception& serializeEx) {
        std::cerr << std::format("Cannot serialize data: {}. Connection denied\n",  serializeEx.what());
        listen();
    }
    catch (const std::exception& ex) {
        std::cerr << std::format("Network erorr: {}", ex.what());
    }
}

bool NetworkController::isNewConnectionValid(const JoinGameMsg& joinmsg)
{
    return joinmsg.secret == kSecretTest;
}

void NetworkController::updateGameSettings(const JoinGameMsg& joinMsg)
{
    m_gameSettings.guestId = joinMsg.guestPlayerId;
    m_gameSettings.guestName = joinMsg.guestPlayerName;
}

} // namespace network

// TODO check how serialization methods will behave after new  attribute, check if client dissconnected
// test client dissconnected, second connection denied, wrong secred and second success,