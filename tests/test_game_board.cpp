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


class gameBoardFixture : public ::testing::Test {
public:
    std::vector<core::GameTileType> testTypes{ core::GameTileType::Grass, core::GameTileType::Grass };
    core::GameBoard testableBoard{ testTypes, 40,  40 };
    std::unique_ptr<UnitManager> unitMng{ std::make_unique<UnitManager>() };
    std::unique_ptr<Unit> unit{ std::make_unique<TankUnit>(UnitIdentifier{ 1 }, TileDistance{7}, Shots{3}) };
    Unit* unitExpected;

        gameBoardFixture() {
        // initialization;
        // can also be done in SetUp()
    }

    void SetUp() {
        //testableBoard.setUnitManager(unitMng);
        

        unit.get()->setPosition(GameTile(4, 4));
        unit.get()->setOwner(PlayerIdentifier{ 1 });
        unit.get()->setBodyRotation(HexVertexNumber{ 3 });
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


// getting upper lines aka setup area test
TEST_F(gameBoardFixture, get3UpperLines)
{
    core::GameBoard testableBoard{ testTypes, 5, 7 };
    auto tilesVec = testableBoard.getUpperLines(3);
    std::vector<core::GameTile> expectedVec{ {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0},
                                          {0,1}, {1,1}, {2,1}, {3,1}, {4,1}, {5,1}, {6,1} ,
                                           {-1,2}, {0,2}, {1,2}, {2,2}, {3,2}, {4,2}, {5,2} };
    EXPECT_EQ(tilesVec, expectedVec);
}

TEST_F(gameBoardFixture, get3BottomLines)
{
    core::GameBoard testableBoard{ testTypes, 5, 7 };
    auto tilesVec = testableBoard.getBottomLines(3);
    std::vector<core::GameTile> expectedVec{ {-1,2}, {0,2}, {1,2}, {2,2}, {3,2}, {4,2}, {5,2},
                                              {-1,3}, {0,3},{1,3}, {2,3}, {3,3}, {4,3}, {5,3},
                                             {-2,4}, {-1, 4}, {0,4} ,{1,4}, {2,4}, {3,4}, {4,4} };
    EXPECT_EQ(tilesVec, expectedVec);
}

TEST_F(gameBoardFixture, get2BottomLines)
{
    core::GameBoard testableBoard{ testTypes, 5, 7 };
    auto tilesVec = testableBoard.getBottomLines(2);
    std::vector<core::GameTile> expectedVec{ {-1,3}, {0,3},{1,3}, {2,3}, {3,3}, {4,3}, {5,3},
                                             {-2,4}, {-1, 4}, {0,4} ,{1,4}, {2,4}, {3,4}, {4,4} };
    EXPECT_EQ(tilesVec, expectedVec);
}

TEST_F(gameBoardFixture, GetMoveAreaCommand) {
    auto cmd{ std::make_unique<GetMoveAreaQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    core::GameEngine gameEngine{testableBoard, std::move(unitMng), PlayerIdentifier{1 }, PlayerIdentifier{2 } };
    cmd.get()->m_unitID = { 1 };
    cmd.get()->m_playerID = { 1 };

    gameEngine.requestMoveArea(cmd.get());
    EXPECT_EQ(1, 1);
}

TEST_F(gameBoardFixture, MoveUnitTo1_0) {
    auto cmdMove{std::make_unique<MoveToAction>() };
    cmdMove.get()->m_unitID = { 1 };
    cmdMove.get()->m_playerID = { 1 };
    core::GameTile expectedPos = core::GameTile(1, 0);
    static_cast<MoveToAction*>(cmdMove.get())->m_destination = expectedPos;


    auto result = unitExpected->moveTo(cmdMove.get(), testableBoard);
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

    auto result = unitExpected->moveTo(cmdMove.get(), testableBoard);
    //EXPECT_EQ(result, true) << "move command failed\n";
    EXPECT_EQ(unitExpected->getPosition(), expectedPos) << "incorrect pos after move\n";
}

TEST_F(gameBoardFixture, MoveUnitTo1_0WithAdjustmentTo1_1) {
    testableBoard.setTileType(GameTile(1, 0), GameTileType::Water);
    testableBoard.setTileType(GameTile(0, 1), GameTileType::Water);

    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove.get()->m_unitID = { 1 };
    cmdMove.get()->m_playerID = { 1 };
    core::GameTile expectedPos = core::GameTile(2, 0);
    cmdMove.get()->m_destination = core::GameTile(1, 0);

    auto result = unitExpected->moveTo(cmdMove.get(), testableBoard);
    //EXPECT_EQ(result, true) << "move command failed\n";
    EXPECT_EQ(unitExpected->getPosition(), expectedPos) << "incorrect pos after move\n";
}

TEST_F(gameBoardFixture, MoveUnitTo1_0WithUnreachableTarget) {
    testableBoard.setTileAccessible(GameTile(1, 0), false);
    testableBoard.setTileAccessible(GameTile(0, 1), false);

    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove.get()->m_unitID = { 1 };
    cmdMove.get()->m_playerID = { 1 };
    core::GameTile expectedPos = core::GameTile(4, 4);
    cmdMove.get()->m_destination = core::GameTile(1, 0);

    auto result = unitExpected->moveTo(cmdMove.get(), testableBoard);
    //EXPECT_EQ(result, false) << "move command should fail\n";
    EXPECT_EQ(unitExpected->getPosition(), expectedPos) << "incorrect pos after move\n";
}
