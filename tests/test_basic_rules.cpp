#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../src/core/unit.h"
#include "../src/core/unit_manager.h"
#include "../src/core/game_rules.h"
#include "../src/core/game_command.h"
#include "../src/core/data_types.h"

#include <ranges>
#include <memory>

using namespace core;

using UnitManagerPtr = std::unique_ptr<core::UnitManager>;

class GameBasicRulesFixture : public ::testing::Test {
public:
    UnitManagerPtr unitMng{ std::make_unique<UnitManager>() };
    std::unique_ptr<Unit> unitFirst{ std::make_unique<BasicTank>(UnitIdentifier{ 1 }, TileDistance{7}) };
    std::unique_ptr<Unit> unitSecond{ std::make_unique<BasicTank>(UnitIdentifier{ 2 }, TileDistance{7}) };
    Unit* unitPtrFirst;
    Unit* unitPtrSecond;
    GameRulesBasic m_gameRules;

    GameBasicRulesFixture() {
        // initialization;
        // can also be done in SetUp()
    }

    void SetUp() {
        unitFirst.get()->setPosition(GameTile(0, 0));
        unitFirst.get()->setOwner(PlayerIdentifier{ 1 });
        unitPtrFirst = unitFirst.get();

        unitSecond.get()->setPosition(GameTile(2, 0));
        unitSecond.get()->setOwner(PlayerIdentifier{ 1 });
        unitPtrSecond = unitSecond.get();
        (*unitMng).addUnit(std::move(unitFirst));
        (*unitMng).addUnit(std::move(unitSecond));
        
        m_gameRules.setUnitManager(unitMng.get());
        m_gameRules.setCurrentPlayer(PlayerIdentifier{ 1 });
        m_gameRules.setStage(GameRulesInterface::GameStage::PlayerIdle);
        // initialization or some code to run before each test
    }

    void TearDown() {
        // code to run after each test;
        // can be used instead of a destructor,
        // but exceptions can be handled in this function only
    }

    ~GameBasicRulesFixture() {
        //resources cleanup, no exceptions allowed
    }

};

TEST_F(GameBasicRulesFixture, selectFirstUnit) {
    auto selectUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    auto result = m_gameRules.selectUnit(selectUnitQuery.get());
    UnitIdentifier expected = { 1 };
    EXPECT_EQ(expected, result) << "Can not select first unit  \n";
}

TEST_F(GameBasicRulesFixture, selectNewUnit) {
    auto selectUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());

    auto selectAnotherUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    auto result = m_gameRules.selectUnit(selectAnotherUnitQuery.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 2 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, disallowNewUnitSelectionAfterAnotherUsed) {
    auto selectUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());
    std::vector<core::GameTile> path{ {0,1} };
    unitPtrFirst->moveTo(path);

    auto selectAnotherUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    m_gameRules.selectUnit(selectAnotherUnitQuery.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 1 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, diselectUnit) {
    auto selectUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());

    auto diselectQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    auto result = m_gameRules.selectUnit(diselectQuery.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 0 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, selectAfterDiselectUnit) {
    auto selectUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());

    auto diselectQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(diselectQuery.get());

    auto selectSecondUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    auto result = m_gameRules.selectUnit(selectSecondUnitQuery.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 2 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, diselectSecondUnit) {
    auto selectUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());

    auto diselectQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(diselectQuery.get());

    auto selectSecondUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    m_gameRules.selectUnit(selectSecondUnitQuery.get());

    auto diselectSecondUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    auto result = m_gameRules.selectUnit(diselectSecondUnitQuery.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 0 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}

TEST_F(GameBasicRulesFixture, disallowDiselectSecondUnit) {
    auto selectUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(selectUnitQuery.get());

    auto diselectQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{1}) };
    m_gameRules.selectUnit(diselectQuery.get());

    auto selectSecondUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    m_gameRules.selectUnit(selectSecondUnitQuery.get());
    std::vector<core::GameTile> path{ {0,1}, {0,2}, {0,3} };
    unitPtrSecond->moveTo(path);

    auto diselectSecondUnitQuery{ std::make_shared<SelectUnitQuery>(PlayerIdentifier{1}, UnitIdentifier{2}) };
    auto result = m_gameRules.selectUnit(diselectSecondUnitQuery.get());

    auto resultUnitId = m_gameRules.getSelectedUnit();
    auto expectedUnitId = UnitIdentifier{ 2 };
    EXPECT_EQ(resultUnitId, expectedUnitId) << "Incorrect selected unit ID  \n";
}