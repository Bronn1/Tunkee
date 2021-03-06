#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "src/core/unit.h"
#include "src/core/unit_manager.h"
#include "src/core/game_rules.h"
#include "src/core/game_command.h"
#include "src/core/data_types.h"
#include "src/core/game_board.h"

#include <ranges>
#include <memory>

using namespace core;

using UnitManagerPtr = std::unique_ptr<core::UnitManager>;

class GameBasicRulesFixture : public ::testing::Test {
public:
    std::vector<core::GameTileType> testTypes{ core::GameTileType::Grass, core::GameTileType::Grass };
    core::GameBoard testableBoard{ testTypes, 40,  40 };
    UnitManagerPtr unitMng{ std::make_unique<UnitManager>() };
    std::unique_ptr<Unit> unitFirst{ std::make_unique<TankUnit>(UnitIdentifier{ 1 }, TileDistance{7}, Shots{3}) };
    std::unique_ptr<Unit> unitSecond{ std::make_unique<TankUnit>(UnitIdentifier{ 2 }, TileDistance{7}, Shots{3}) };
    std::unique_ptr<Unit> unitThird{ std::make_unique<TankUnit>(UnitIdentifier{ 3 }, TileDistance{7}, Shots{3}) };
    std::unique_ptr<Unit> unitFourth{ std::make_unique<TankUnit>(UnitIdentifier{ 4 }, TileDistance{7}, Shots{3}) };
    Unit* unitPtrFirst;
    Unit* unitPtrSecond;
    Unit* unitPtrThird;
    Unit* unitPtrFourth;
    GameRulesBasic m_gameRules;

    GameBasicRulesFixture() {
        // initialization;
        // can also be done in SetUp()
    }

    void SetUp() {
        //first player units
        unitFirst.get()->setPosition(GameTile(0, 0));
        unitFirst.get()->setOwner(PlayerIdentifier{ 1 });
        unitPtrFirst = unitFirst.get();

        unitSecond.get()->setPosition(GameTile(2, 0));
        unitSecond.get()->setOwner(PlayerIdentifier{ 1 });
        unitPtrSecond = unitSecond.get();

        //second player units
        unitThird.get()->setPosition(GameTile(8, 2));
        unitThird.get()->setOwner(PlayerIdentifier{ 2 });
        unitPtrThird = unitThird.get();

        unitFourth.get()->setPosition(GameTile(8, 3));
        unitFourth.get()->setOwner(PlayerIdentifier{ 2 });
        unitPtrFourth = unitFourth.get();

        unitMng->addUnit(std::move(unitFirst));
        unitMng->addUnit(std::move(unitSecond));
        unitMng->addUnit(std::move(unitThird));
        unitMng->addUnit(std::move(unitFourth));
        
        m_gameRules.setUnitManager(unitMng.get());
        m_gameRules.setCurrentPlayer(PlayerIdentifier{ 1 });
        m_gameRules.setPlayer(PlayerIdentifier{ 1 });
        m_gameRules.setPlayer(PlayerIdentifier{ 2 });
        m_gameRules.setStage(GameRulesInterface::GameStage::ActionPhase);
        m_gameRules.setActiveUnits(PlayerIdentifier{ 1 });
        m_gameRules.setActiveUnits(PlayerIdentifier{ 2 });
    }

    void TearDown() {}

    ~GameBasicRulesFixture() {}
};

TEST_F(GameBasicRulesFixture, selectFirstUnit) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    auto result = m_gameRules.selectUnit(selectUnitQuery.get());
    UnitIdentifier expected = { 1 };
    EXPECT_EQ(expected, result) << "Can not select first unit  \n";
}

// we should be able to select another unit until we have used some activity on one unit
TEST_F(GameBasicRulesFixture, selectNewUnit) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());

    auto selectAnotherUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    auto result = m_gameRules.selectUnit(selectAnotherUnitQuery.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 2 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

// game shouldn't allow us to change unit after one has been used(used some of his action state moving, shooting , rotation)
TEST_F(GameBasicRulesFixture, disallowNewUnitSelectionAfterAnotherUsed) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());

    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unitPtrFirst->getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 1);
    unitPtrFirst->moveTo(cmdMove.get(), testableBoard);

    auto selectAnotherUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    m_gameRules.selectUnit(selectAnotherUnitQuery.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 1 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, diselectUnit) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());

    auto diselectQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    auto result = m_gameRules.selectUnit(diselectQuery.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 0 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, selectAfterDiselectUnit) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());

    auto diselectQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(diselectQuery.get());

    auto selectSecondUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    auto result = m_gameRules.selectUnit(selectSecondUnitQuery.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 2 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, diselectSecondUnit) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());

    auto diselectQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(diselectQuery.get());

    auto selectSecondUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    m_gameRules.selectUnit(selectSecondUnitQuery.get());

    auto diselectSecondUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    auto result = m_gameRules.selectUnit(diselectSecondUnitQuery.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 0 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

// we cant diselect unit after performing move
TEST_F(GameBasicRulesFixture, disallowDiselectSecondUnit) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());

    auto diselectQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(diselectQuery.get());

    auto selectSecondUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    m_gameRules.selectUnit(selectSecondUnitQuery.get());
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unitPtrSecond->getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unitPtrSecond->moveTo(cmdMove.get(), testableBoard);

    auto diselectSecondUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    auto result = m_gameRules.selectUnit(diselectSecondUnitQuery.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 2 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

// player made some actions in game and decided to end up his phase of a turn 
TEST_F(GameBasicRulesFixture, switchFirstPLayerPhase) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unitPtrFirst->getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unitPtrFirst->moveTo(cmdMove.get(), testableBoard);

    auto finishActionPhase{ std::make_unique<FinishActionPhase>() };
    finishActionPhase->m_playerID = PlayerIdentifier{ 1 };
    m_gameRules.nextActionPhase(finishActionPhase.get());

    auto resultPlayer = m_gameRules.getCurrentPlayer();
    auto expectedPlayer = PlayerIdentifier{ 2 };
    EXPECT_EQ(resultPlayer, expectedPlayer) << "Incorrect  player ID on next action phase \n";

    auto resultStage = m_gameRules.getCurrentStage();
    auto expectedStage = GameRulesInterface::GameStage::ActionPhase;
    EXPECT_EQ(resultStage, expectedStage) << "Incorrect  game stage \n";

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 0 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

// here we will use all units to make end of current turn
TEST_F(GameBasicRulesFixture, endOFTurn) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unitPtrFirst->getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unitPtrFirst->moveTo(cmdMove.get(), testableBoard);

    auto finishActionPhase{ std::make_unique<FinishActionPhase>() };
    finishActionPhase->m_playerID = PlayerIdentifier{ 1 };
    m_gameRules.nextActionPhase(finishActionPhase.get());
    finishActionPhase->m_playerID = PlayerIdentifier{ 2 };
    m_gameRules.nextActionPhase(finishActionPhase.get());
    finishActionPhase->m_playerID = PlayerIdentifier{ 1 };
    m_gameRules.nextActionPhase(finishActionPhase.get());
    finishActionPhase->m_playerID = PlayerIdentifier{ 2 };
    m_gameRules.nextActionPhase(finishActionPhase.get());

    auto resultPlayer = m_gameRules.getCurrentPlayer();
    auto expectedPlayer = PlayerIdentifier{1 };
    EXPECT_EQ(resultPlayer, expectedPlayer) << "Incorrect  player ID on next turn \n";

    auto resultStage = m_gameRules.getCurrentStage();
    auto expectedStage = GameRulesInterface::GameStage::TurnEnd;
    EXPECT_EQ(resultStage, expectedStage) << "Incorrect  game stage \n";

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 0 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, switchSecondPLayerPhase) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unitPtrFirst->getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unitPtrFirst->moveTo(cmdMove.get(), testableBoard);

    auto finishActionPhase{ std::make_unique<FinishActionPhase>() };
    finishActionPhase->m_playerID = PlayerIdentifier{ 1 };
    m_gameRules.nextActionPhase(finishActionPhase.get());
    finishActionPhase->m_playerID = PlayerIdentifier{ 2 };
    m_gameRules.nextActionPhase(finishActionPhase.get());

    auto resultPlayer = m_gameRules.getCurrentPlayer();
    auto expectedPlayer = PlayerIdentifier{ 1 };
    EXPECT_EQ(resultPlayer, expectedPlayer) << "Incorrect  player ID on next turn \n";

    auto resultStage = m_gameRules.getCurrentStage();
    auto expectedStage = GameRulesInterface::GameStage::ActionPhase;
    EXPECT_EQ(resultStage, expectedStage) << "Incorrect  game stage \n";

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 0 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, TryToSelectUnitAfterPhaseSwitch) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unitPtrFirst->getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unitPtrFirst->moveTo(cmdMove.get(), testableBoard);

    auto finishActionPhase{ std::make_unique<FinishActionPhase>() };
    finishActionPhase->m_playerID = PlayerIdentifier{ 1 };
    m_gameRules.nextActionPhase(finishActionPhase.get());
    auto selectUnitQuery2{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{2}, UnitIdentifier{3}) };
    m_gameRules.selectUnit(selectUnitQuery2.get());
    cmdMove->m_unitID = { unitPtrThird->getID() };
    cmdMove->m_destination = GameTile(0, 3);
    unitPtrThird->moveTo(cmdMove.get(), testableBoard);

    auto resultPlayer = m_gameRules.getCurrentPlayer();
    auto expectedPlayer = PlayerIdentifier{ 2 };
    EXPECT_EQ(resultPlayer, expectedPlayer) << "Incorrect  player ID on next turn \n";

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 3 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, disallowSelectUnitAfterPhaseSwitch) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unitPtrFirst->getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unitPtrFirst->moveTo(cmdMove.get(), testableBoard);

    auto finishActionPhase{ std::make_unique<FinishActionPhase>() };
    finishActionPhase->m_playerID = PlayerIdentifier{ 1 };
    m_gameRules.nextActionPhase(finishActionPhase.get());
    m_gameRules.selectUnit(selectUnitQuery.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 0 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, disallowSelectUnitWithWrongPlayerId) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unitPtrFirst->getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unitPtrFirst->moveTo(cmdMove.get(), testableBoard);

    auto finishActionPhase{ std::make_unique<FinishActionPhase>() };
    finishActionPhase->m_playerID = PlayerIdentifier{ 1 };
    m_gameRules.nextActionPhase(finishActionPhase.get());
    auto selectUnitQuery2{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    m_gameRules.selectUnit(selectUnitQuery2.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 0 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, TryToSelectUnitAfterTwoPhaseSwitch) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unitPtrFirst->getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unitPtrFirst->moveTo(cmdMove.get(), testableBoard);

    auto finishActionPhase{ std::make_unique<FinishActionPhase>() };
    finishActionPhase->m_playerID = PlayerIdentifier{ 1 };
    m_gameRules.nextActionPhase(finishActionPhase.get());
    finishActionPhase->m_playerID = PlayerIdentifier{ 2 };
    m_gameRules.nextActionPhase(finishActionPhase.get());
    auto selectUnitQuery2{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    m_gameRules.selectUnit(selectUnitQuery2.get());
    cmdMove->m_destination = GameTile(8, 5);
    unitPtrThird->moveTo(cmdMove.get(), testableBoard);

    auto resultPlayer = m_gameRules.getCurrentPlayer();
    auto expectedPlayer = PlayerIdentifier{ 1 };
    EXPECT_EQ(resultPlayer, expectedPlayer) << "Incorrect  player ID on next turn \n";

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 2 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, TryToSelectUsedUnit) {
    auto selectUnitQuery{ std::make_unique<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unitPtrFirst->getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unitPtrFirst->moveTo(cmdMove.get(), testableBoard);

    auto finishActionPhase{ std::make_unique<FinishActionPhase>() };
    finishActionPhase->m_playerID = PlayerIdentifier{ 1 };
    m_gameRules.nextActionPhase(finishActionPhase.get());
    finishActionPhase->m_playerID = PlayerIdentifier{ 2 };
    m_gameRules.nextActionPhase(finishActionPhase.get());

    m_gameRules.selectUnit(selectUnitQuery.get());


    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 0 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}
