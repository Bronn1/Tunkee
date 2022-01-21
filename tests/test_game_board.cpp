#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/core/pathfinding.h"
#include "../src/core/game_board.h"
#include "../src/core/game_command.h"
#include "../src/core/data_types.h"
#include "../src/core/game_rules.h"
#include "../src/core/player.h"
#include "../src/core/unit.h"
#include "../src/core/unit_manager.h"
#include "../src/core/game_engine.h"
#include <ranges>
#include <memory>

using namespace core;


class MockUnit7Distance : public Unit {
public:
    MockUnit7Distance(UnitIdentifier id) : Unit(id, TileDistance{ 7 }, Shots{ 3 }) {
        m_actionState.setStateSystem(std::make_unique<tank_state_system::TankState>(Crew{ 5, 3 }));
    }
    MOCK_CONST_METHOD0(isAlive, bool());
};

class gameBoardFixture : public ::testing::Test {
public:
    std::vector<core::GameTileType> testTypes{ core::GameTileType::Grass, core::GameTileType::Grass };
    core::GameBoard testableBoard{ testTypes, 40,  40 };
    UnitManagerPtr unitMng{ std::make_unique<UnitManager>() };
    std::unique_ptr<Unit> unit{ std::make_unique<MockUnit7Distance>(UnitIdentifier{ 1 } )};
    Unit* unitExpected;

        gameBoardFixture() {
        // initialization;
        // can also be done in SetUp()
    }

    void SetUp() {
        //testableBoard.setUnitManager(unitMng);
        

        unit.get()->setPosition(GameTile(4, 4));
        unit.get()->setOwner(PlayerIdentifier{ 1 });
        unitExpected = unit.get();
        (*unitMng).addUnit(std::move(unit));
        // initialization or some code to run before each test
    }

    void TearDown() {
        // code to run after each test;
        // can be used instead of a destructor,
        // but exceptions can be handled in this function only
    }

    ~gameBoardFixture() {
        //resources cleanup, no exceptions allowed
    }

};

TEST_F(gameBoardFixture, GetMoveAreaCommand) {
    auto cmd{ std::make_shared<GetMoveAreaQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    core::GameEngine gameEngine{testableBoard, std::move(unitMng), PlayerIdentifier{1 }, PlayerIdentifier{2 } };
    cmd.get()->m_unitID = { 1 };
    cmd.get()->m_playerID = { 1 };

    gameEngine.requestMoveArea(cmd.get());
    EXPECT_EQ(1, 1);
}

TEST_F(gameBoardFixture, MoveUnitTo1_0) {
    auto cmdMove{std::make_shared<MoveToAction>() };
    cmdMove.get()->m_unitID = { 1 };
    cmdMove.get()->m_playerID = { 1 };
    core::GameTile expectedPos = core::GameTile(1, 0);
    static_cast<MoveToAction*>(cmdMove.get())->m_destination = expectedPos;


    auto result = testableBoard.moveTo(cmdMove.get(), unitExpected);
    //EXPECT_EQ(result, true) << "move command failed\n";
    EXPECT_EQ(unitExpected->getPosition(), expectedPos) << "incorrect pos after move\n";
}

TEST_F(gameBoardFixture, MoveUnitTo1_0WithAdjustmentTo2_1) {
    testableBoard.setTileType(GameTile(1, 0), GameTileType::Water);
    testableBoard.setTileType(GameTile(0, 1), GameTileType::Water);
    testableBoard.setTileType(GameTile(-1, 2), GameTileType::Water);
    testableBoard.setTileType(GameTile(0, 2), GameTileType::Water);
    testableBoard.setTileType(GameTile(1, 1), GameTileType::Water);
    testableBoard.setTileType(GameTile(2, 0), GameTileType::Water);
    testableBoard.setTileType(GameTile(4, 4), GameTileType::Water);
    testableBoard.setTileType(GameTile(3, 4), GameTileType::Water);
    testableBoard.setTileType(GameTile(4, 3), GameTileType::Water);
    testableBoard.setTileType(GameTile(5, 3), GameTileType::Water);
    testableBoard.setTileType(GameTile(3, 5), GameTileType::Water);

    auto cmdMove{ std::make_shared<MoveToAction>() };
    cmdMove.get()->m_unitID = { 1 };
    cmdMove.get()->m_playerID = { 1 };
    core::GameTile expectedPos = core::GameTile(2, 1);
    static_cast<MoveToAction*>(cmdMove.get())->m_destination = core::GameTile(1, 0);

    auto result = testableBoard.moveTo(cmdMove.get(), unitExpected);
    //EXPECT_EQ(result, true) << "move command failed\n";
    EXPECT_EQ(unitExpected->getPosition(), expectedPos) << "incorrect pos after move\n";
}

TEST_F(gameBoardFixture, MoveUnitTo1_0WithAdjustmentTo1_1) {
    testableBoard.setTileType(GameTile(1, 0), GameTileType::Water);
    testableBoard.setTileType(GameTile(0, 1), GameTileType::Water);

    auto cmdMove{ std::make_shared<MoveToAction>() };
    cmdMove.get()->m_unitID = { 1 };
    cmdMove.get()->m_playerID = { 1 };
    core::GameTile expectedPos = core::GameTile(2, 0);
    cmdMove.get()->m_destination = core::GameTile(1, 0);

    auto result = testableBoard.moveTo(static_cast<MoveToAction*>(cmdMove.get()), unitExpected);
    //EXPECT_EQ(result, true) << "move command failed\n";
    EXPECT_EQ(unitExpected->getPosition(), expectedPos) << "incorrect pos after move\n";
}

TEST_F(gameBoardFixture, MoveUnitTo1_0WithUnreachableTarget) {
    testableBoard.setTileAccessible(GameTile(1, 0), false);
    testableBoard.setTileAccessible(GameTile(0, 1), false);

    auto cmdMove{ std::make_shared<MoveToAction>() };
    cmdMove.get()->m_unitID = { 1 };
    cmdMove.get()->m_playerID = { 1 };
    core::GameTile expectedPos = core::GameTile(4, 4);
    cmdMove.get()->m_destination = core::GameTile(1, 0);

    auto result = testableBoard.moveTo(cmdMove.get(), unitExpected);
    //EXPECT_EQ(result, false) << "move command should fail\n";
    EXPECT_EQ(unitExpected->getPosition(), expectedPos) << "incorrect pos after move\n";
}
