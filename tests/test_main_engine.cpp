#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/core/pathfinding.h"
#include "../src/core/game_engine.h"

class MyGameEngineRulesFixture : public ::testing::Test {
public:
    std::vector<core::GameTileType> testTypes{ core::GameTileType::Grass, core::GameTileType::Grass };
    core::GameBoard testableBoard{ testTypes, 40,  40 };
    UnitManagerPtr unitMng{ std::make_unique<UnitManager>() };
    core::GameEngine gameEngine{testableBoard, std::move(unitMng), PlayerIdentifier{1 }, PlayerIdentifier{2 } };

    MyGameEngineRulesFixture() {
        // initialization;
        // can also be done in SetUp()
    }

    void SetUp() {
        
        // initialization or some code to run before each test
    }

    void TearDown() {
        // code to run after each test;
        // can be used instead of a destructor,
        // but exceptions can be handled in this function only
    }

    ~MyGameEngineRulesFixture() {
        //resources cleanup, no exceptions allowed
    }

};

TEST_F(MyGameEngineRulesFixture, UnitTest2) {
    EXPECT_EQ(1, 1);
    
    EXPECT_TRUE(true) << "diagnostic message";;
}