#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/core/pathfinding.h"
#include "../src/core/game_board.h"
#include "../src/core/game_command.h"
#include "../src/core/data_types.h"
#include "../src/core/game_rules.h"
#include "../src/core/player.h"
#include "../src/core/unit.h"
#include "../src/core/events.h"
#include "../src/core/game_engine.h"
#include <ranges>
#include <memory>

using namespace core;

class observerFixture : public ::testing::Test {
public:
    std::vector<core::GameTileType> testTypes{ core::GameTileType::Grass, core::GameTileType::Grass };
    core::GameBoard testableBoard{ testTypes, 40,  40 };
    UnitManagerPtr unitMng{ std::make_unique<UnitManager>() };
    

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
    MockUnit(UnitIdentifier id, TileDistance dis) : Unit(id, TileDistance{ dis }, Shots{ 3 }) {
        setStateSystem(std::make_unique<tank_state_system::TankState>(Crew{ 5, 3 }));
    }
    MOCK_CONST_METHOD0(isAlive, bool());

};


class MockObserver : public events::Observer<GameEngine> {
public:
    MockObserver() = default;

    std::vector<core::GameTile> m_result;
    int m_halfSize;
};

TEST_F(observerFixture, observerMoveArea18_6) {
    auto cmd{ std::make_shared<GetMoveAreaQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    int expectedFull = 18;
    int expectedHalf = 6;


    auto unit = std::make_unique<MockUnit>(UnitIdentifier{ 1 }, TileDistance{ 2 });

    unit.get()->setPosition(GameTile(2, 2));
    unit.get()->setOwner(PlayerIdentifier{ 1 });
    (*unitMng).addUnit(std::move(unit));
    //std::shared_ptr<events::Observer> observer{ std::make_shared<MockObserver>() };
    //testableBoard.addObserver(cmd.get()->m_unitID, observer);

    core::GameEngine gameEngine{testableBoard, std::move(unitMng), PlayerIdentifier{1 }, PlayerIdentifier{2 } };
     gameEngine.requestMoveArea(cmd.get());
    //int resultSize = std::size(moveArea.moveArea);
    //int resultFirstLayer = moveArea.firstLayerSize;
   // EXPECT_EQ(resultSize, expectedFull);
    //EXPECT_EQ(resultFirstLayer, expectedHalf);
}

TEST_F(observerFixture, observerMoveArea9_5) {
    testableBoard.setTileAccessible(GameTile(1, 1), false);
    testableBoard.setTileAccessible(GameTile(3, 0), false);
    auto cmd{ std::make_shared<GetMoveAreaQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    int expectedFull = 9;
    int expectedHalf = 5;

    cmd.get()->m_unitID = { 1 };
    cmd.get()->m_playerID = { 1 };

    auto unit = std::make_unique<MockUnit>(UnitIdentifier{ 1 }, TileDistance{ 3 });

    unit.get()->setPosition(GameTile(0, 0));
    unit.get()->setOwner(PlayerIdentifier{ 1 });
    (*unitMng).addUnit(std::move(unit));

    core::GameEngine gameEngine{testableBoard, std::move(unitMng), PlayerIdentifier{1 }, PlayerIdentifier{2 } };
    gameEngine.requestMoveArea(cmd.get());
    //int resultSize = std::size(moveArea.moveArea);
    //int resultFirstLayer = moveArea.firstLayerSize;
    //EXPECT_EQ(resultSize, expectedFull);
    //EXPECT_EQ(resultFirstLayer, expectedHalf);
}

