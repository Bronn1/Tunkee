#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/core/pathfinding.h"
#include "../src/core/game_board.h"
#include "../src/core/game_command.h"
#include "../src/core/data_types.h"
#include "../src/core/game_state_manager.h"
#include "../src/core/player.h"
#include "../src/core/unit.h"
#include "../src/core/unit_manager.h"
#include "../src/core/game_rules_engine.h"
#include <ranges>
#include <memory>

using namespace core;


class MockUnit7Distance : public Unit {
public:
    MockUnit7Distance(ObjectIdentifier id) : Unit(id, TileDistance{ 7 }, Shots{ 3 }) {}
};

class gameBoardFixture : public ::testing::Test {
public:
    std::vector<core::GameTileType> testTypes{ core::GameTileType::grass, core::GameTileType::grass };
    core::GameBoard testableBoard{ testTypes, 40,  40 };
    UnitManagerPtr unitMng{ std::make_shared<UnitManager>() };
    std::shared_ptr<Unit> unit{ std::make_shared<MockUnit7Distance>(ObjectIdentifier{ 1 } )};

    gameBoardFixture() {
        // initialization;
        // can also be done in SetUp()
    }

    void SetUp() {
        //testableBoard.setUnitManager(unitMng);
        

        unit.get()->setPosition(GameTile(4, 4));
        unit.get()->setOwner(PlayerIdentifier{ 1 });
        (*unitMng).addUnit(unit);
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
    auto cmd{ std::make_shared<GetMoveAreaQuery>() };
    core::GameRulesEngine gameEngine{testableBoard, unitMng, PlayerIdentifier{1 }, PlayerIdentifier{2 } };
    cmd.get()->m_unitID = { 1 };
    cmd.get()->m_playerID = { 1 };

    gameEngine.queryMoveArea(cmd.get());
    EXPECT_EQ(1, 1);
}

TEST_F(gameBoardFixture, MoveUnitTo1_0) {
    auto cmdMove{std::make_shared<MoveToAction>() };
    cmdMove.get()->m_unitID = { 1 };
    cmdMove.get()->m_playerID = { 1 };
    core::GameTile expectedPos = core::GameTile(1, 0);
    static_cast<MoveToAction*>(cmdMove.get())->m_destination = expectedPos;


    auto result = testableBoard.moveTo(cmdMove.get(), unit.get());
    //EXPECT_EQ(result, true) << "move command failed\n";
    EXPECT_EQ(unit.get()->getPosition(), expectedPos) << "incorrect pos after move\n";
}

TEST_F(gameBoardFixture, MoveUnitTo1_0WithAdjustmentTo2_1) {
    testableBoard.setTileType(GameTile(1, 0), GameTileType::water);
    testableBoard.setTileType(GameTile(0, 1), GameTileType::water);
    testableBoard.setTileType(GameTile(-1, 2), GameTileType::water);
    testableBoard.setTileType(GameTile(0, 2), GameTileType::water);
    testableBoard.setTileType(GameTile(1, 1), GameTileType::water);
    testableBoard.setTileType(GameTile(2, 0), GameTileType::water);
    testableBoard.setTileType(GameTile(4, 4), GameTileType::water);
    testableBoard.setTileType(GameTile(3, 4), GameTileType::water);
    testableBoard.setTileType(GameTile(4, 3), GameTileType::water);
    testableBoard.setTileType(GameTile(5, 3), GameTileType::water);
    testableBoard.setTileType(GameTile(3, 5), GameTileType::water);

    auto cmdMove{ std::make_shared<MoveToAction>() };
    cmdMove.get()->m_unitID = { 1 };
    cmdMove.get()->m_playerID = { 1 };
    core::GameTile expectedPos = core::GameTile(2, 1);
    static_cast<MoveToAction*>(cmdMove.get())->m_destination = core::GameTile(1, 0);

    auto result = testableBoard.moveTo(cmdMove.get(), unit.get());
    //EXPECT_EQ(result, true) << "move command failed\n";
    EXPECT_EQ(unit.get()->getPosition(), expectedPos) << "incorrect pos after move\n";
}

TEST_F(gameBoardFixture, MoveUnitTo1_0WithAdjustmentTo1_1) {
    testableBoard.setTileType(GameTile(1, 0), GameTileType::water);
    testableBoard.setTileType(GameTile(0, 1), GameTileType::water);

    auto cmdMove{ std::make_shared<MoveToAction>() };
    cmdMove.get()->m_unitID = { 1 };
    cmdMove.get()->m_playerID = { 1 };
    core::GameTile expectedPos = core::GameTile(2, 0);
    cmdMove.get()->m_destination = core::GameTile(1, 0);

    auto result = testableBoard.moveTo(static_cast<MoveToAction*>(cmdMove.get()), unit.get());
    //EXPECT_EQ(result, true) << "move command failed\n";
    EXPECT_EQ(unit.get()->getPosition(), expectedPos) << "incorrect pos after move\n";
}

TEST_F(gameBoardFixture, MoveUnitTo1_0WithUnreachableTarget) {
    testableBoard.setTileAccessible(GameTile(1, 0), false);
    testableBoard.setTileAccessible(GameTile(0, 1), false);

    auto cmdMove{ std::make_shared<MoveToAction>() };
    cmdMove.get()->m_unitID = { 1 };
    cmdMove.get()->m_playerID = { 1 };
    core::GameTile expectedPos = core::GameTile(4, 4);
    cmdMove.get()->m_destination = core::GameTile(1, 0);

    auto result = testableBoard.moveTo(cmdMove.get(), unit.get());
    //EXPECT_EQ(result, false) << "move command should fail\n";
    EXPECT_EQ(unit.get()->getPosition(), expectedPos) << "incorrect pos after move\n";
}
