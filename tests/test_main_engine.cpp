#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "src/core/pathfinding.h"
#include "src/core/game_engine.h"

#include <ranges>


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

// generates a lot of uniforms ints in interval from 0  to 20
// every value in this inteval should appear roughly the same amount of times
TEST_F(MyGameEngineRulesFixture, testUnifirmDiceDistribution) {
    DamageCalculator damageCalculator{};
    std::vector< int> uniformDiceRes(21, 0);
    //uniformDiceRes.reserve(20);
    for (int i : std::views::iota(0, 1000000))
    //for (int i = 0; i <= 1000000; i++)
    {
        uniformDiceRes[damageCalculator.rollDiceWithFaces(20)]++;
    }

    for(int i  = 0; i <= 20; i++)
    {
        std::cout << i << ": " << uniformDiceRes[i] << "\n";
    }
}