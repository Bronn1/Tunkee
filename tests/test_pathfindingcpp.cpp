#include "gtest/gtest.h"

#include "../src/core/pathfinding.h"
#include "../src/core/game_board.h"
#include <unordered_set>
#include <ranges>

using namespace core;

class pathfindingFixture : public ::testing::Test {
public:
    std::vector<core::GameTileType> testTypes{ core::GameTileType::Grass, core::GameTileType::Grass };
    core::GameBoard testableBoard{ testTypes, 40,  40 };

    pathfindingFixture() {
        
        // initialization;
        // can als o be done in SetUp()
    }

    void SetUp() {
        //testableBoard = core::GameBoard(testTypes, 5, 7);
        // initialization or some code to run before each test
    }

    void TearDown() {
        // code to run after each test;
        // can be used instead of a destructor,
        // but exceptions can be handled in this function only
    }

    ~pathfindingFixture() {
        //resources cleanup, no exceptions allowed
    }

};

TEST_F(pathfindingFixture, strightLineDistance) {
    int expectedDistance = 3;
    auto begin = GameTile(0, 2);
    auto end = GameTile(0, -1);
    EXPECT_EQ(pathfinding::getDistance(begin, end), expectedDistance);
}

TEST_F(pathfindingFixture, curveLineDistance) {
    int expectedDistance = 6;
    auto begin = GameTile(0, 2);
    auto end = GameTile(2, -4);
    EXPECT_EQ(pathfinding::getDistance(begin, end), expectedDistance);

    int expectedDistanceLong = 8;
    auto beginL = GameTile(-1, 4);
    auto endL = GameTile(0, -4);
    EXPECT_EQ(pathfinding::getDistance(beginL, endL), expectedDistanceLong);
}

TEST_F(pathfindingFixture, LineDistanceZero) {
    int expectedDistance = 0;
    auto begin = GameTile(0, 2);
    auto end = GameTile(0, 2);
    EXPECT_EQ(pathfinding::getDistance(begin, end), expectedDistance);
}

//u can find screeshot of the  boar in examples folder for better understading of coordinate system
TEST_F(pathfindingFixture, get6Neighbors) {
    auto source = GameTile(3, 1);
    std::vector <GameTile> expected = { GameTile(3,0),GameTile(4,0), GameTile(4,1),
                                             GameTile(3,2), GameTile(2,2), GameTile(2,1) };
    auto result = pathfinding::getNeighbors(testableBoard, source);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "Vectors x and y are of unequal length";
    for (int i: std::views::iota(0, size)) {
        EXPECT_EQ(result[i], expected[i]) << "Vectors x and y differ at index " << i;
    }
}

TEST_F(pathfindingFixture, getNeighborsOnTheEdge) {
    auto source = GameTile(0, 1);
    std::vector <GameTile> expected = { GameTile(0,0),GameTile(1,0), GameTile(1,1),
                                         GameTile(0,2),GameTile(-1,2)};
    auto result = pathfinding::getNeighbors(testableBoard, source);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "Vectors x and y are of unequal length";
    for (int i : std::views::iota(0, size)) {
        EXPECT_EQ(result[i], expected[i]) << "Vectors x and y differ at index " << i;
    }
}

TEST_F(pathfindingFixture, getAvailableAreaWithMovement1) {
    auto source = GameTile(3, 1);
    std::unordered_set < GameTile> expected = { GameTile(3,0), GameTile(4,1),
                                             GameTile(3,2), GameTile(2,2), GameTile(2,1) };
    testableBoard.setTileType(GameTile(4, 0), GameTileType::Water); // this tile become unavailable cuz movement only 1

    auto result = pathfinding::getAvailableArea(testableBoard, source, TileDistance{ 1 });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected) ;
}

TEST_F(pathfindingFixture, getAvailableAreaWithMovement3) {
    auto source = GameTile(1, 1);
    std::unordered_set <GameTile> expected = { GameTile(0,0), GameTile(1,0),GameTile(2,0), GameTile(3,0), GameTile(4,0), GameTile(0,1),
                                             GameTile(2,1), GameTile(3,1), GameTile(4,1), GameTile(-1,2),
                                             GameTile(0,2), GameTile(1,2), GameTile(2,2),
                                              GameTile(3,2), GameTile(-1,3), GameTile(0,3) ,
                                                 GameTile(1,3), GameTile(2,3), GameTile(-2,4), GameTile(-1,4) };
    testableBoard.setTileType(GameTile(0, 0), GameTileType::Water); //
    testableBoard.setTileAccessible(GameTile(0, 4), false);
    testableBoard.setTileAccessible(GameTile(1, 4), false);

    auto result = pathfinding::getAvailableArea(testableBoard, source, TileDistance{ 3 });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}

TEST_F(pathfindingFixture, getAvailableAreaWithMovement3MoreBlocked) {
    auto source = GameTile(2, 2);
    std::unordered_set <GameTile> expected = { GameTile(1,2),GameTile(1,1), GameTile(0,1),
                                             GameTile(2,1), GameTile(3,1), GameTile(4,1), GameTile(4,2),
                                             GameTile(0,3), GameTile(-1,3), GameTile(-1, 4), GameTile(0,4) };
    testableBoard.setTileType(GameTile(3, 3), GameTileType::Water); //
    testableBoard.setTileAccessible(GameTile(1, 0), false); 
    testableBoard.setTileAccessible(GameTile(2, 0), false);
    testableBoard.setTileAccessible(GameTile(3, 0), false);
    testableBoard.setTileAccessible(GameTile(4, 0), false);  
    testableBoard.setTileAccessible(GameTile(5, 0), false);
    testableBoard.setTileAccessible(GameTile(5, 1), false);
    testableBoard.setTileAccessible(GameTile(3, 2), false); 
    testableBoard.setTileAccessible(GameTile(2, 3), false);
    testableBoard.setTileAccessible(GameTile(1, 3), false);
    testableBoard.setTileAccessible(GameTile(0, 2), false); 

    auto result = pathfinding::getAvailableArea(testableBoard, source, TileDistance{ 3 });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}

TEST_F(pathfindingFixture, getAvailableAreaWithMovement2) {
    auto source = GameTile(-1, 2);
    std::unordered_set < GameTile> expected = { GameTile(0,0), GameTile(1,0),
                                             GameTile(0,1), GameTile(1,1),
                                              GameTile(-1,3), GameTile(0,3),
                                             GameTile(-2,4)};
    //testableBoard.setTileType(GameTile(5, 1), GameTileType::Water); //
    testableBoard.setTileAccessible(GameTile(-1, 4), false);
    testableBoard.setTileAccessible(GameTile(0, 2), false);

    auto result = pathfinding::getAvailableArea(testableBoard, source, TileDistance{ 2 });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}

TEST_F(pathfindingFixture, getAvailableAreaWithMovement4) {
    auto source = GameTile(0, 0);
    std::unordered_set < GameTile> expected = { GameTile(1,0),GameTile(0,1), GameTile(1,1), GameTile(-1,2),
                                               GameTile(0,2), GameTile(-1,3), GameTile(0,3), GameTile(1,3),
                                               GameTile(-2,4), GameTile(-1,4), GameTile(0,4) };
    testableBoard.setTileType(GameTile(1, 0), GameTileType::Water); //
    testableBoard.setTileType(GameTile(1, 1), GameTileType::Water);
    testableBoard.setTileAccessible(GameTile(2, 0), false);
    testableBoard.setTileAccessible(GameTile(3, 0), false);
    testableBoard.setTileAccessible(GameTile(4, 0), false);
    testableBoard.setTileAccessible(GameTile(2, 1), false);
    testableBoard.setTileAccessible(GameTile(3, 1), false);
    testableBoard.setTileAccessible(GameTile(1, 2), false);
    testableBoard.setTileAccessible(GameTile(2, 2), false);

    auto result = pathfinding::getAvailableArea(testableBoard, source, TileDistance{ 4 });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}

TEST_F(pathfindingFixture, getAvailableAreaEmpty) {
    auto source = GameTile(3, 1);
    std::unordered_set < GameTile> expected = { };
    testableBoard.setTileType(GameTile(4, 0), GameTileType::Water); // this tile become unavailable cuz movement only 1
    testableBoard.setTileType(GameTile(3, 0), GameTileType::Water);
    testableBoard.setTileType(GameTile(4, 1), GameTileType::Water);
    testableBoard.setTileType(GameTile(3, 2), GameTileType::Water);
    testableBoard.setTileType(GameTile(2, 2), GameTileType::Water);
    testableBoard.setTileType(GameTile(2, 1), GameTileType::Water);

    auto result = pathfinding::getAvailableArea(testableBoard, source, TileDistance{ 1 });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}

TEST_F(pathfindingFixture, getShortestPath15) {
    auto source = GameTile(0, 0);
    auto destination = GameTile(-1, 4);
    std::vector < GameTile> expected = { GameTile(1,0),GameTile(2,0), GameTile(3,0), GameTile(4,0), GameTile(5,0),
                                               GameTile(6,0), GameTile(6,1), GameTile(5,2), GameTile(4,2),
                                               GameTile(3,3), GameTile(2,3), GameTile(1,4), GameTile(0,4),
                                               GameTile(-1,4)};
    testableBoard.setTileAccessible(GameTile(1, 1), false); // 
    testableBoard.setTileAccessible(GameTile(2, 1), false);
    testableBoard.setTileAccessible(GameTile(3, 1), false);
    testableBoard.setTileAccessible(GameTile(4, 1), false); // 
    testableBoard.setTileAccessible(GameTile(5, 1), false);
    testableBoard.setTileAccessible(GameTile(-1, 2), false);
    testableBoard.setTileAccessible(GameTile(0, 2), false); // 
    testableBoard.setTileAccessible(GameTile(1, 2), false);
    testableBoard.setTileAccessible(GameTile(2, 2), false);
    testableBoard.setTileAccessible(GameTile(3, 2), false); // 
    testableBoard.setTileAccessible(GameTile(1, 3), false);
    testableBoard.setTileType(GameTile(4, 3), GameTileType::Water);
    testableBoard.setTileType(GameTile(3, 3), GameTileType::Water);
    testableBoard.setTileType(GameTile(2, 4), GameTileType::Water);
    testableBoard.setTileType(GameTile(3, 4), GameTileType::Water);

    auto result = pathfinding::getShortestPath(testableBoard, source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}

TEST_F(pathfindingFixture, getShortestPath15TwoDiffPaths) {
    auto source = GameTile(0, 0);
    auto destination = GameTile(-1, 4);
    std::vector < GameTile> expected = { GameTile(1,0),GameTile(2,0), GameTile(3,0), GameTile(4,0), GameTile(5,0),
                                               GameTile(6,0), GameTile(6,1), GameTile(5,2), GameTile(4,3),
                                               GameTile(3,3), GameTile(2,3), GameTile(1,4), GameTile(0,4),
                                               GameTile(-1,4) };
    testableBoard.setTileAccessible(GameTile(1, 1), false); // 
    testableBoard.setTileAccessible(GameTile(2, 1), false);
    testableBoard.setTileAccessible(GameTile(3, 1), false);
    testableBoard.setTileAccessible(GameTile(4, 1), false); // 
    testableBoard.setTileAccessible(GameTile(5, 1), false);
    testableBoard.setTileAccessible(GameTile(-1, 2), false);
    testableBoard.setTileAccessible(GameTile(0, 2), false); // 
    testableBoard.setTileAccessible(GameTile(1, 2), false);
    testableBoard.setTileAccessible(GameTile(2, 2), false);
    testableBoard.setTileAccessible(GameTile(3, 2), false); // 
    testableBoard.setTileAccessible(GameTile(1, 3), false);
    testableBoard.setTileType(GameTile(3, 3), GameTileType::Water);
    testableBoard.setTileType(GameTile(2, 4), GameTileType::Water);
    testableBoard.setTileType(GameTile(3, 4), GameTileType::Water);

    auto result = pathfinding::getShortestPath(testableBoard, source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}

TEST_F(pathfindingFixture, getShortestPath9) {
    auto source = GameTile(0, 0);
    auto destination = GameTile(0, 4);
    std::vector < GameTile> expected = { GameTile(0,1),GameTile(0,2), GameTile(-1,3), GameTile(-1,4), GameTile(0,4)};
    testableBoard.setTileAccessible(GameTile(1, 1), false); // 
    testableBoard.setTileAccessible(GameTile(2, 1), false);
    testableBoard.setTileAccessible(GameTile(3, 1), false);
    testableBoard.setTileAccessible(GameTile(4, 1), false); // 
    testableBoard.setTileAccessible(GameTile(5, 1), false);
    testableBoard.setTileAccessible(GameTile(-1, 2), false);
    testableBoard.setTileAccessible(GameTile(0, 3), false); // 
    testableBoard.setTileAccessible(GameTile(1, 2), false);
    testableBoard.setTileAccessible(GameTile(2, 2), false);
    testableBoard.setTileAccessible(GameTile(3, 2), false); // 
    testableBoard.setTileAccessible(GameTile(1, 3), false);
    testableBoard.setTileType(GameTile(0, 1), GameTileType::Test3);
    testableBoard.setTileType(GameTile(0, 2), GameTileType::Test3);
    testableBoard.setTileType(GameTile(2, 4), GameTileType::Water);
    testableBoard.setTileType(GameTile(3, 3), GameTileType::Water);
    testableBoard.setTileType(GameTile(4, 3), GameTileType::Water);

    auto result = pathfinding::getShortestPath(testableBoard, source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}

/*//only for big maps like 1000 on 1000
TEST_F(pathfindingFixture, getShortestPath1150) {
    auto source = GameTile(0, 0);
    auto destination = GameTile(450, 700);
    std::vector < GameTile> expected = { GameTile(0,1),GameTile(0,2), GameTile(-1,3), GameTile(-1,4), GameTile(0,4), GameTile(0,4) };
    std::cout << testableBoard.contains(GameTile(500, 700)) << "fff\m";
    testableBoard.setTileAccessible(GameTile(1, 1), false); // 
    testableBoard.setTileAccessible(GameTile(2, 1), false);
    testableBoard.setTileAccessible(GameTile(3, 1), false);
    testableBoard.setTileAccessible(GameTile(4, 1), false); // 
    testableBoard.setTileAccessible(GameTile(5, 1), false);
    testableBoard.setTileAccessible(GameTile(-1, 2), false);
    testableBoard.setTileAccessible(GameTile(0, 3), false); // 
    testableBoard.setTileAccessible(GameTile(1, 2), false);
    testableBoard.setTileAccessible(GameTile(2, 2), false);
    testableBoard.setTileAccessible(GameTile(3, 2), false); // 
    testableBoard.setTileAccessible(GameTile(1, 3), false);
    testableBoard.setTileType(GameTile(0, 1), GameTileType::Test3);
    testableBoard.setTileType(GameTile(0, 2), GameTileType::Test3);
    testableBoard.setTileType(GameTile(2, 4), GameTileType::Water);
    testableBoard.setTileType(GameTile(3, 3), GameTileType::Water);
    testableBoard.setTileType(GameTile(4, 3), GameTileType::Water);

    auto result = pathfinding::getShortestPath(testableBoard, source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}*/

TEST_F(pathfindingFixture, getShortestPath13) {
    auto source = GameTile(0, 0);
    auto destination = GameTile(0, 4);
    std::vector < GameTile> expected = { GameTile(1,0),GameTile(2,0), GameTile(3,0), GameTile(4,0), GameTile(5,0),
                                               GameTile(6,0), GameTile(6,1), GameTile(5,2), GameTile(4,2),
                                               GameTile(3,3), GameTile(2,3), GameTile(1,4), GameTile(0,4) };
    testableBoard.setTileAccessible(GameTile(1, 1), false); // 
    testableBoard.setTileAccessible(GameTile(2, 1), false);
    testableBoard.setTileAccessible(GameTile(3, 1), false);
    testableBoard.setTileAccessible(GameTile(4, 1), false); // 
    testableBoard.setTileAccessible(GameTile(5, 1), false);
    testableBoard.setTileAccessible(GameTile(-1, 2), false);
    testableBoard.setTileAccessible(GameTile(0, 3), false); // 
    testableBoard.setTileAccessible(GameTile(1, 2), false);
    testableBoard.setTileAccessible(GameTile(2, 2), false);
    testableBoard.setTileAccessible(GameTile(3, 2), false); // 
    testableBoard.setTileAccessible(GameTile(1, 3), false);
    testableBoard.setTileType(GameTile(0, 1), GameTileType::Test5);
    testableBoard.setTileType(GameTile(0, 2), GameTileType::Test5);
    testableBoard.setTileType(GameTile(-1, 3), GameTileType::Test3);
    testableBoard.setTileType(GameTile(2, 4), GameTileType::Water);
    testableBoard.setTileType(GameTile(3, 3), GameTileType::Water);
    testableBoard.setTileType(GameTile(4, 3), GameTileType::Water);

    auto result = pathfinding::getShortestPath(testableBoard, source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}

TEST_F(pathfindingFixture, getShortestPath4StraightLine) {
    auto source = GameTile(0, 0);
    auto destination = GameTile(0, 4);
    std::vector < GameTile> expected = { GameTile(0,1),GameTile(0,2), GameTile(0,3), GameTile(0,4) };


    auto result = pathfinding::getShortestPath(testableBoard, source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}

TEST_F(pathfindingFixture, getShortestPathUnaccessible) {
    auto source = GameTile(0, 0);
    auto destination = GameTile(0, 4);
    std::vector < GameTile> expected = { };
    testableBoard.setTileAccessible(GameTile(-1, 2), false); // 
    testableBoard.setTileAccessible(GameTile(0, 2), false);
    testableBoard.setTileAccessible(GameTile(1, 1), false);
    testableBoard.setTileAccessible(GameTile(2, 0), false);


    auto result = pathfinding::getShortestPath(testableBoard, source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}

TEST_F(pathfindingFixture, getShortestPath1) {
    auto source = GameTile(0, 0);
    auto destination = GameTile(0, 1);
    std::vector < GameTile> expected = { GameTile(0, 1)  };

    auto result = pathfinding::getShortestPath(testableBoard, source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}

TEST_F(pathfindingFixture, getShortestPath0) {
    auto source = GameTile(0, 0);
    auto destination = GameTile(0, 0);
    std::vector < GameTile> expected = { };

    auto result = pathfinding::getShortestPath(testableBoard, source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}

TEST_F(pathfindingFixture, getStrightLine) {
    std::vector<core::GameTileType> testTypes1{ core::GameTileType::Grass, core::GameTileType::Grass };
    core::GameBoard testableBoard1{ testTypes1, 5, 7 };
    auto source = GameTile(0, 0);
    auto destination = GameTile(0, 4);
    std::vector < GameTile> expected = { GameTile(0,1),GameTile(0,2), GameTile(0,3), GameTile(0,4) };

    auto result = pathfinding::drawLine( source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}

TEST_F(pathfindingFixture, getLongLine) {
    std::vector<core::GameTileType> testTypes1{ core::GameTileType::Grass, core::GameTileType::Grass };
    core::GameBoard testableBoard1{ testTypes1, 5, 7 };
    auto source = GameTile(0, 0);
    auto destination = GameTile(4, 4);
    std::vector < GameTile> expected = { GameTile(1,0),GameTile(1,1), GameTile(2,1), GameTile(2,2), 
                                         GameTile(3,2),GameTile(3,3), GameTile(4,3), GameTile(4,4) };

    auto result = pathfinding::drawLine(source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}

TEST_F(pathfindingFixture, getLine7) {
    std::vector<core::GameTileType> testTypes1{ core::GameTileType::Grass, core::GameTileType::Grass };
    core::GameBoard testableBoard1{ testTypes1, 5, 7 };
    auto source = GameTile(5, 0);
    auto destination = GameTile(-2, 4);
    std::vector < GameTile> expected = { GameTile(4,1),GameTile(3,1), GameTile(2,2), GameTile(1,2),
                                         GameTile(0,3),GameTile(-1,3), GameTile(-2,4) };

    auto result = pathfinding::drawLine(source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}

TEST_F(pathfindingFixture, getLine2) {
    std::vector<core::GameTileType> testTypes1{ core::GameTileType::Grass, core::GameTileType::Grass };
    core::GameBoard testableBoard1{ testTypes1, 5, 7 };
    auto source = GameTile(1, 0);
    auto destination = GameTile(3, 0);
    std::vector < GameTile> expected = { GameTile(2,0),GameTile(3,0) };

    auto result = pathfinding::drawLine(source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}

TEST_F(pathfindingFixture, getLine1) {
    std::vector<core::GameTileType> testTypes1{ core::GameTileType::Grass, core::GameTileType::Grass };
    core::GameBoard testableBoard1{ testTypes1, 5, 7 };
    auto source = GameTile(4, 0);
    auto destination = GameTile(3, 0);
    std::vector < GameTile> expected = { GameTile(3,0) };

    auto result = pathfinding::drawLine(source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}

TEST_F(pathfindingFixture, getLineZero) {
    std::vector<GameTileType> testTypes1{ core::GameTileType::Grass, core::GameTileType::Grass };
    core::GameBoard testableBoard1{ testTypes1, 5, 7 };
    auto source = GameTile(1, 0);
    auto destination = GameTile(1, 0);
    std::vector < GameTile> expected = {};

    auto result = pathfinding::drawLine(source, destination);
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    EXPECT_EQ(result, expected);
}


TEST_F(pathfindingFixture, getLineofSightWithoutBarriersAt0_2) {
    auto source = GameTile(0, 2);
    std::unordered_set < GameTile> expected = { GameTile(0,3),GameTile(1,2), GameTile(0,4), GameTile(1,3), GameTile(2,2),
                                               GameTile(0,5), GameTile(1,4), GameTile(2,3), GameTile(3,2) };


    auto result = pathfinding::getLineOfSightWithoutObstacles(testableBoard, source, TileDistance{ 3 }, HexVertexNumber{ 2 });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << " result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}

TEST_F(pathfindingFixture, getLineofSightWithoutBarriersAt6_1) {
    auto source = GameTile(6, 1);
    std::unordered_set < GameTile> expected = { GameTile(5,1),GameTile(5,2), GameTile(4,1), GameTile(4,2), GameTile(4,3),
                                               GameTile(3,1), GameTile(3,2), GameTile(3,3), GameTile(3,4) , GameTile(2,1),
                                               GameTile(2,2),GameTile(2,3), GameTile(2,4), GameTile(2,5), GameTile(1,1),
                                               GameTile(1,2), GameTile(1,3), GameTile(1,4), GameTile(1,5) , GameTile(1,6) };


    auto result = pathfinding::getLineOfSightWithoutObstacles(testableBoard, source, TileDistance{ 5 }, HexVertexNumber{ 4 });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << " result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}

TEST_F(pathfindingFixture, getLineofSightWithoutBarriersAt3_1) {
    auto source = GameTile(3, 1);
    std::unordered_set < GameTile> expected = { GameTile(2,2),GameTile(3,2), GameTile(1,3), GameTile(2,3), GameTile(3,3),
                                               GameTile(0,4), GameTile(1,4), GameTile(2,4), GameTile(3,4) };


    auto result = pathfinding::getLineOfSightWithoutObstacles(testableBoard, source, TileDistance{ 3 }, HexVertexNumber{ 3  });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << " result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}

TEST_F(pathfindingFixture, getLineofSightWithoutBarriersAt0_3) {
    auto source = GameTile(0, 3);
    std::unordered_set < GameTile> expected = { GameTile(1,2),GameTile(1,3), GameTile(2,1), GameTile(2,2), GameTile(2,3),
                                               GameTile(3,0), GameTile(3,1), GameTile(3,2), GameTile(3,3),
                                               GameTile(4,0), GameTile(4,1), GameTile(4,2), GameTile(4,3), };


    auto result = pathfinding::getLineOfSightWithoutObstacles(testableBoard, source, TileDistance{ 4 }, HexVertexNumber{ 1 });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << " result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}

TEST_F(pathfindingFixture, getLineofSightWithoutBarriersAt1_4) {
    auto source = GameTile(1, 4);
    std::unordered_set < GameTile> expected = { GameTile(1,3),GameTile(2,3), GameTile(1,2), GameTile(2,2), GameTile(3,2),
                                               GameTile(1,1), GameTile(2,1), GameTile(3,1), GameTile(4,1),
                                               GameTile(1,0), GameTile(2,0), GameTile(3,0), GameTile(4,0), GameTile(5,0), };


    auto result = pathfinding::getLineOfSightWithoutObstacles(testableBoard, source, TileDistance{ 5 }, HexVertexNumber{ 0 });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << " result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}

TEST_F(pathfindingFixture, getLineofSightWithoutBarriersAt4_3) {
    auto source = GameTile(4, 3);
    std::unordered_set < GameTile> expected = { GameTile(3,3),GameTile(4,2), GameTile(2,3), GameTile(3,2), GameTile(4,1),
                                               GameTile(1,3), GameTile(2,2), GameTile(3,1), GameTile(4,0) };


    auto result = pathfinding::getLineOfSightWithoutObstacles(testableBoard, source, TileDistance{3 }, HexVertexNumber{ 5 });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << " result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}

TEST_F(pathfindingFixture, getAvailableAreaWithMovement3ROTATION) {
    auto source = GameTile(2, 2);
    std::unordered_set <GameTile> expected = { GameTile(-1,2), GameTile(0,2),GameTile(1,2), GameTile(3,2), GameTile(4,2), GameTile(5,2),
                                              GameTile(0,3), GameTile(1,3), GameTile(2,3), GameTile(3,3), GameTile(4,3),
                                               GameTile(0,4), GameTile(1,4), GameTile(2,4), GameTile(3,4), GameTile(2,5),
                                                GameTile(2,1), GameTile(3,1),  GameTile(1,1), GameTile(4,1),
                                                GameTile(2,0),  GameTile(3,0), GameTile(4,0) };
    testableBoard.setTileType(GameTile(-1, 3), GameTileType::Water); //
    testableBoard.setTileType(GameTile(-1, 4), GameTileType::Water);
    testableBoard.setTileType(GameTile(0, 4), GameTileType::Water);
    testableBoard.setTileType(GameTile(1, 4), GameTileType::Water);
    testableBoard.setTileType(GameTile(1, 5), GameTileType::Water);

    auto result = pathfinding::getAvailableAreaWithRotation(testableBoard, source, TileDistance{ 3 }, HexVertexNumber{ 3 });
    //auto result = pathfinding::getAvailableArea(testableBoard, source, TileDistance{ 3 });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}

TEST_F(pathfindingFixture, getAvailableAreaWithMovement1ROTATION) {
    auto source = GameTile(4, 2);
    std::unordered_set <GameTile> expected = { GameTile(3,2), GameTile(3,3),GameTile(4,3), GameTile(5,2),  };

    auto result = pathfinding::getAvailableAreaWithRotation(testableBoard, source, TileDistance{ 1 }, HexVertexNumber{ 3  });
    //auto result = pathfinding::getAvailableArea(testableBoard, source, TileDistance{ 3 });
    int size = result.size();

    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}

TEST_F(pathfindingFixture, ROTATIONgetAvailableAreaWithMovement3) {
    auto source = GameTile(1, 3);
    std::unordered_set <GameTile> expected = { GameTile(3,3), GameTile(2,2), GameTile(2,3), GameTile(2,4),
                                              GameTile(1,2), GameTile(1,4), GameTile(0,2), GameTile(0,3), GameTile(0,4),
                                               GameTile(0,5), GameTile(-1,3), GameTile(-1,4), GameTile(- 1,5) };
    testableBoard.setTileType(GameTile(1, 2), GameTileType::Test3); //
    testableBoard.setTileType(GameTile(2, 1), GameTileType::Test3);
    testableBoard.setTileType(GameTile(1, 4), GameTileType::Test3);
    testableBoard.setTileType(GameTile(2, 3), GameTileType::Water);
    testableBoard.setTileType(GameTile(2, 2), GameTileType::Water);
    testableBoard.setTileType(GameTile(3, 2), GameTileType::Water);
    testableBoard.setTileType(GameTile(3, 1), GameTileType::Water);

    auto result = pathfinding::getAvailableAreaWithRotation(testableBoard, source, TileDistance{ 3 }, HexVertexNumber{ 1 });
    //auto result = pathfinding::getAvailableArea(testableBoard, source, TileDistance{ 3 });
    int size = result.size();
    //
    ASSERT_EQ(result.size(), expected.size()) << "  result and expected should have same length";
    std::unordered_set < GameTile> resultToSet(std::begin(result), std::end(result));
    EXPECT_EQ(resultToSet, expected);
}



