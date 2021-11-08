#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/core/pathfinding.h"
#include "../src/core/game_board.h"
#include "../src/core/game_command.h"
#include "../src/core/data_types.h"
#include "../src/core/game_state_manager.h"
#include "../src/core/player.h"
#include "../src/core/unit.h"
#include "../src/core/events.h"
#include "../src/core/main_engine.h"
#include <ranges>
#include <memory>

using namespace core;

class observerFixture : public ::testing::Test {
public:
    std::vector<core::GameTileType> testTypes{ core::GameTileType::grass, core::GameTileType::grass };
    core::GameBoard testableBoard{ testTypes, 40,  40 };
    UnitManagerPtr unitMng{ std::make_shared<UnitManager>() };
    

    observerFixture() {
        // initialization;
        // can also be done in SetUp()
    }

    void SetUp() {
        //testableBoard.setUnitManager(unitMng);
        // initialization or some code to run before each test
    }

    void TearDown() {
        // code to run after each test;
        // can be used instead of a destructor,
        // but exceptions can be handled in this function only
    }

    ~observerFixture() {
        //resources cleanup, no exceptions allowed
    }

};

class MockUnit : public Unit {
public:
    MockUnit(ObjectIdentifier id, TileDistance dis) : Unit(id, TileDistance{ dis }, Shots{ 3 }) {}

};


class MockObserver : public events::Observer {
public:
    MockObserver() = default;

    void handleTileEvent(const std::string& eventName,const  std::vector<core::GameTile>& tiles, int firstLayerMoveSize) override{
        m_result =  tiles;
        m_halfSize = firstLayerMoveSize;
    }
    std::vector<core::GameTile> m_result;
    int m_halfSize;
};

TEST_F(observerFixture, observerMoveArea18_6) {
    std::shared_ptr<GameQuery> cmd{ std::make_shared<GetMoveAreaQuery>() };
    int expectedFull = 18;
    int expectedHalf = 6;

    cmd.get()->m_unitID = { 1 };
    cmd.get()->m_playerID = { 1 };

    std::shared_ptr<Unit> unit = std::make_shared<MockUnit>(ObjectIdentifier{ 1 }, TileDistance{ 2 });

    unit.get()->setPosition(GameTile(2, 2));
    unit.get()->setOwner(PlayerIdentifier{ 1 });
    (*unitMng).addUnit(unit);
    std::shared_ptr<events::Observer> observer{ std::make_shared<MockObserver>() };
    testableBoard.addObserver(cmd.get()->m_unitID, observer);

    core::MainEngine gameEngine{ testableBoard, unitMng, PlayerIdentifier{ 1 }, PlayerIdentifier{ 2 } };
    gameEngine.executeQuery(cmd);
    int resultSize = std::size(static_cast<MockObserver*>(observer.get())->m_result);
    int resultSizeHalf = static_cast<MockObserver*>(observer.get())->m_halfSize;
    EXPECT_EQ(resultSize, expectedFull);
    EXPECT_EQ(resultSizeHalf, expectedHalf);
    //EXPECT_EQ(result, true) << "move command failed\n";
    //EXPECT_EQ(unit.get()->getPosition(), expectedPos) << "incorrect pos after move\n";
}

TEST_F(observerFixture, observerMoveArea9_5) {
    testableBoard.setTileAccessible(GameTile(1, 1), false);
    testableBoard.setTileAccessible(GameTile(3, 0), false);
    std::shared_ptr<GameQuery> cmd{ std::make_shared<GetMoveAreaQuery>() };
    int expectedFull = 9;
    int expectedHalf = 5;

    cmd.get()->m_unitID = { 1 };
    cmd.get()->m_playerID = { 1 };

    std::shared_ptr<Unit> unit = std::make_shared<MockUnit>(ObjectIdentifier{ 1 }, TileDistance{ 3 });

    unit.get()->setPosition(GameTile(0, 0));
    unit.get()->setOwner(PlayerIdentifier{ 1 });
    (*unitMng).addUnit(unit);
    std::shared_ptr<events::Observer> observer{ std::make_shared<MockObserver>() };
    testableBoard.addObserver(cmd.get()->m_unitID, observer);

    core::MainEngine gameEngine{ testableBoard, unitMng, PlayerIdentifier{ 1 }, PlayerIdentifier{ 2 } };
    gameEngine.executeQuery(cmd);
    int resultSize = std::size(static_cast<MockObserver*>(observer.get())->m_result);
    int resultSizeHalf = static_cast<MockObserver*>(observer.get())->m_halfSize;
    EXPECT_EQ(resultSize, expectedFull);
    EXPECT_EQ(resultSizeHalf, expectedHalf);
    //EXPECT_EQ(result, true) << "move command failed\n";
    //EXPECT_EQ(unit.get()->getPosition(), expectedPos) << "incorrect pos after move\n";
}

