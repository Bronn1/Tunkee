#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <WinSDKVer.h>
#define _WIN32_WINNT _WIN32_WINNT_WIN10
#include <SDKDDKVer.h>
#include "src/network/network_controller.cpp"

#include <string_view>
#include <asio/ip/tcp.hpp>
#include <vector>
#include <asio/io_context.hpp>
#include <thread>

#include <cereal/archives/binary.hpp>
#include <sstream>

constexpr std::string_view kHostIp = "127.0.0.1";
constexpr std::string_view kStdPort = "3999";
constexpr size_t kSecretTesting = 123210432848324343;
const std::string kGuestName = "testGuest";
const std::string kHostName = "testHost";
constexpr PlayerIdentifier kHostId{ 1 };
constexpr PlayerIdentifier kGuestId{ 123 };

using namespace network;

class NetworkFixture : public ::testing::Test
{
public:
    NetworkFixture()
    {
    }

    void SetUp()
    {
    }

    void TearDown()
    {
    }

    ~NetworkFixture(){
        
    }


    settings::GameSettings m_launchSettings{ {}, kHostName, "", settings::MapSize{40, 40}, PlayerIdentifier{1}, PlayerIdentifier{2}, setup::Money{1000},
                             settings::GameType::Multiplayer , core::GameBoardType::Plain };
    NetworkController net_controller{ m_launchSettings,  std::atoi(kStdPort.data()) };
    
private:



};


TEST_F(NetworkFixture, testPlayerJoined_network)
{
    // not the best test performace prob
    auto  serverThread = std::thread([this]() {
        auto events = this->net_controller.run();
        std::cout << std::format("Thread {}\n", events);
    });
    // set up asio first
    asio::io_context  ctx{};
    auto  hostEndpoint = *asio::ip::tcp::resolver(ctx).resolve(kHostIp, kStdPort);
    asio::ip::tcp::socket sock(ctx, hostEndpoint.endpoint().protocol());
    asio::error_code ec;
    sock.connect(hostEndpoint.endpoint(),ec );

    network::JoinGameMsg joinMsg{  kGuestName, kSecretTesting, kGuestId };
    int bytes = 1024; 
    std::stringstream serializeOutputStream;
    cereal::BinaryOutputArchive serializeToBinary(serializeOutputStream);
    serializeToBinary(joinMsg);

    auto n = sock.send(asio::buffer(serializeOutputStream.str()));
    if (n == 0) {
        std::cout << ec.message() << std::endl;
        EXPECT_EQ(false, true);
    }
    std::cout << "bytes sent: " << n << '\n';
    const size_t kBytes = 512u;
    std::string receivedData(kBytes, '\0');
    network::GameInfoMsg receivedMsg;
    
    auto bytes_read = sock.receive(asio::buffer(receivedData, bytes));
    receivedData.resize(bytes_read);
    std::stringstream serializeStream(receivedData);
    cereal::BinaryInputArchive serializeTo(serializeStream);
    try {
        serializeTo(receivedMsg);
    }
    catch (const cereal::Exception& ex) {
        std::cout << "Cannot serialize data: " << ex.what() << '\n';
    }
  
    serverThread.join();
    ctx.poll();
    EXPECT_EQ(receivedMsg.secret, kSecretTesting);
    EXPECT_EQ(receivedMsg.settings.guestId, kGuestId);
    EXPECT_EQ(receivedMsg.settings.hostName, kHostName);
    EXPECT_EQ(receivedMsg.settings.guestName, kGuestName);
}
