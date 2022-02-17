#include "gtest/gtest.h"
#include "gmock/gmock.h"


#include "src/core/unit.h"
#include "src/core/unit.cpp"
#include "src/core/game_tile.h"
#include "src/core/game_board.h"

using namespace core;

class UnitFixture : public ::testing::Test
{
public:
    // So unit can do per turn  either 5 move or 3 shots or 2 shots and 3 move
    TankUnit unit{ UnitIdentifier{ 1 }, TileDistance{5}, Shots{3} };
    TankUnit unitWithEvenaction{ UnitIdentifier{ 1 }, TileDistance{4}, Shots{6} };
    std::vector<core::GameTileType> testTypes{ core::GameTileType::Grass, core::GameTileType::Grass };
    core::GameBoard testableBoard{ testTypes, 40,  40 };

    UnitFixture()
    {
    }

    void SetUp()
    {
        GameTile pos(0,0);
        unit.setPosition(pos);
        unitWithEvenaction.setPosition(pos);
        unit.setUnitRotation(HexVertexNumber{ 3 });
    }


    void TearDown() {

    }

    ~UnitFixture() {
    }
};

TEST_F(UnitFixture, zeroActionsAfterMoveMove)
{
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unit.moveTo(cmdMove.get(), testableBoard);
    cmdMove->m_destination = GameTile(3, 3);
    //std::vector<GameTile> secondActionPath{ GameTile(1,3), GameTile(2,3), GameTile(3,3) }; // only 2 steps left
    unit.moveTo(cmdMove.get(), testableBoard);
    GameTile expectedPosition = GameTile(2, 3);
    bool expectedActionsLeft = false;
    TileDistance expectedMoveLeft = { 0 };
    Shots expectedShotleft = { 0 };
    
    EXPECT_EQ(unit.getPosition(), expectedPosition) << "Unit should have zero actions left after had used whole move points  \n";
    EXPECT_EQ(unit.canMove(), expectedActionsLeft) << "Unit should have zero actions left after had used whole move points  \n";
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft) << "Unit should have zero actions left after had used whole move points  \n";
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft) << "Unit should have zero actions left after had used whole move points  \n";
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft) << "Unit should have zero actions left after had used whole move points  \n";
}

TEST_F(UnitFixture, 1ActionAfterMove)
{
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unit.moveTo(cmdMove.get(), testableBoard);

    GameTile expectedPosition = GameTile(0, 3);
    bool expectedActionsLeft = true;
    TileDistance expectedMoveLeft = { 2 };
    Shots expectedShotleft = { 2 };

    EXPECT_EQ(unit.getPosition(), expectedPosition) << "Unit should have one action left after had used half of move points  \n";
    EXPECT_EQ(unit.canMove(), expectedActionsLeft) << "Unit should have one action left after had used half of move points  \n";
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft) << "Unit should have one action left after had used half of move points  \n";
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft) << "Unit should have one action left after had used half of move points  \n";
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft) << "Unit should have one action left after had used half of move points  \n";
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft) << "Unit should have one shoot left \n";
}

TEST_F(UnitFixture, 1ActionAndMove1AfterMove2Tiles)
{
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 2);
    unit.moveTo(cmdMove.get(), testableBoard);
    GameTile expectedPosition = GameTile(0, 2);
    bool expectedActionsLeft = true;
    TileDistance expectedMoveLeft = { 3 };
    Shots expectedShotleft = { 2 };

    EXPECT_EQ(unit.getPosition(), expectedPosition) << "Unit should have one action left after had used half of move points  \n";
    EXPECT_EQ(unit.canMove(), expectedActionsLeft) << "Unit should have one action left after had used half of move points  \n";
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft) << "Unit should have one action left after had used half of move points  \n";
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft) << "Unit should have one action left after had used half of move points  \n";
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft) << "Unit should have one action left after had used half of move points  \n";
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft) << "Unit should have one shoot left \n";
}

TEST_F(UnitFixture, oneShotLeftAfterMoveShot)
{
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unit.moveTo(cmdMove.get(), testableBoard);
    Shots shot{ 1 };
    unit.shoot(shot);
    bool expectedActionsLeft = true;
    bool expectedMoveAction = false;
    TileDistance expectedMoveLeft = { 0 };
    Shots expectedShotleft = { 1 };

    EXPECT_EQ(unit.canMove(), expectedMoveAction) << "Unit can not move after one move action and one shoot action  \n";
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft) << "Unit should have one shoot left \n";
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft) << "Unit should have one shoot left \n";
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft) << "Unit can not move after one move action and one shoot action  \n";
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft) << "Unit should have one shoot left \n";
}

TEST_F(UnitFixture, move1Move1Sequentially)
{
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 1);
    unit.moveTo(cmdMove.get(), testableBoard);
    cmdMove->m_destination = GameTile(0, 2);
    unit.moveTo(cmdMove.get(), testableBoard);
    bool expectedActionsLeft = true;
    bool expectedMoveAction = true;
    TileDistance expectedMoveLeft = { 3 };
    Shots expectedShotleft = { 2 };

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

TEST_F(UnitFixture, move1Move1Move1Sequentially)
{
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 1);
    unit.moveTo(cmdMove.get(), testableBoard);
    cmdMove->m_destination = GameTile(0, 2);
    unit.moveTo(cmdMove.get(), testableBoard);
    cmdMove->m_destination = GameTile(0, 3);
    unit.moveTo(cmdMove.get(), testableBoard);
    bool expectedActionsLeft = true;
    bool expectedMoveAction = true;
    TileDistance expectedMoveLeft = { 2 };
    Shots expectedShotleft = { 2 };

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

TEST_F(UnitFixture, shot1shot1Sequentially)
{
    Shots shot{ 1 };
    unit.shoot(shot);
    unit.shoot(shot);

    bool expectedActionsLeft = true;
    bool expectedMoveAction = true;
    TileDistance expectedMoveLeft = { 3 };
    Shots expectedShotleft = { 1 };

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

TEST_F(UnitFixture, shot1Move1Sequentially)
{
    Shots shot{ 1 };
    unit.shoot(shot);

    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 2);
    unit.moveTo(cmdMove.get(), testableBoard);

    bool expectedActionsLeft = true;
    bool expectedMoveAction = true;
    TileDistance expectedMoveLeft = { 1 };
    Shots expectedShotleft = { 1 };

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

TEST_F(UnitFixture, zeroActionsAfterMoveShots)
{
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unit.moveTo(cmdMove.get(), testableBoard);

    Shots shot{ 2 };
    unit.shoot(shot);
    GameTile expectedPosition = GameTile(2, 3);
    bool expectedActionsLeft = false;
    bool expectedMoveAction = false;
    TileDistance expectedMoveLeft = { 0 };
    Shots expectedShotleft = { 0 };

    EXPECT_EQ(unit.canMove(), expectedMoveAction) << "Unit should have zero actions  after one move action and one shoot action  \n";
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft) << "Unit should have zero actions  after one move action and one shoot action  \n";
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft) << "Unit should have zero actions  after one move action and one shoot action  \n";
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft) << "Unit should have zero actions  after one move action and one shoot action  \n";
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft) << "Unit should have zero actions  after one move action and one shoot action  \n";
}

TEST_F(UnitFixture, actionAfterOneShot)
{
    Shots shot{ 1 };
    unit.shoot(shot);
    bool expectedActionsLeft = true;
    bool expectedMoveAction = true;
    TileDistance expectedMoveLeft = { 3 };
    Shots expectedShotleft = { 2 };

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft) ;
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft) ;
}

TEST_F(UnitFixture, actionAfterTwoShots)
{
    Shots shot{ 2 };
    unit.shoot(shot);
    bool expectedActionsLeft = true;
    bool expectedMoveAction = true;
    TileDistance expectedMoveLeft = { 3 };
    Shots expectedShotleft = { 1 };

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

TEST_F(UnitFixture, zeroActionsAfterFullShots)
{
    Shots shot{ 3 };
    unit.shoot(shot);
    bool expectedActionsLeft = false;
    bool expectedMoveAction = false;
    TileDistance expectedMoveLeft = { 0 };
    Shots expectedShotleft = { 0 };

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

TEST_F(UnitFixture, actionAfterTwoShotsEvenAmount)
{
    Shots shot{ 2 };
    unitWithEvenaction.shoot(shot);
    bool expectedActionsLeft = true;
    bool expectedMoveAction = true;
    TileDistance expectedMoveLeft = { 2 };
    Shots expectedShotleft = { 4 };

    EXPECT_EQ(unitWithEvenaction.canMove(), expectedMoveAction);
    EXPECT_EQ(unitWithEvenaction.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unitWithEvenaction.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unitWithEvenaction.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unitWithEvenaction.getRemainingShots(), expectedShotleft);
}

TEST_F(UnitFixture, OneMoveAfterMoveEvenAmount)
{
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 3);
    unitWithEvenaction.moveTo(cmdMove.get(), testableBoard);

    GameTile expectedPosition = GameTile(0, 3);
    bool expectedActionsLeft = true;
    TileDistance expectedMoveLeft = { 1 };
    Shots expectedShotleft = { 0 };

    EXPECT_EQ(unitWithEvenaction.getPosition(), expectedPosition);
    EXPECT_EQ(unitWithEvenaction.canMove(), expectedActionsLeft);
    EXPECT_EQ(unitWithEvenaction.canShoot(), false);
    EXPECT_EQ(unitWithEvenaction.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unitWithEvenaction.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unitWithEvenaction.getRemainingShots(), expectedShotleft);
}

TEST_F(UnitFixture, OneActionAfterMoveEvenAmount)
{
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 2);
    unitWithEvenaction.moveTo(cmdMove.get(), testableBoard);

    GameTile expectedPosition = GameTile(0, 2);
    bool expectedActionsLeft = true;
    TileDistance expectedMoveLeft = { 2 };
    Shots expectedShotleft = { 3 };

    EXPECT_EQ(unitWithEvenaction.getPosition(), expectedPosition) ;
    EXPECT_EQ(unitWithEvenaction.canMove(), expectedActionsLeft) ;
    EXPECT_EQ(unitWithEvenaction.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unitWithEvenaction.hasActionLeft(), expectedActionsLeft) ;
    EXPECT_EQ(unitWithEvenaction.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unitWithEvenaction.getRemainingShots(), expectedShotleft) ;
}

TEST_F(UnitFixture, move1Move1SequentiallyEven)
{
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 1);
    unitWithEvenaction.moveTo(cmdMove.get(), testableBoard);
    cmdMove->m_destination = GameTile(0, 2);
    unitWithEvenaction.moveTo(cmdMove.get(), testableBoard);

    bool expectedActionsLeft = true;
    bool expectedMoveAction = true;
    TileDistance expectedMoveLeft = { 2 };
    Shots expectedShotleft = { 3 };

    EXPECT_EQ(unitWithEvenaction.canMove(), expectedMoveAction);
    EXPECT_EQ(unitWithEvenaction.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unitWithEvenaction.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unitWithEvenaction.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unitWithEvenaction.getRemainingShots(), expectedShotleft);
}


TEST_F(UnitFixture, unitGunRotationClockwise)
{
    unit.setUnitRotation(HexVertexNumber(2));
    unit.setGunRotation(Angle{ 91.1 });
    HexVertexNumber expectedVertex{ 4 };
    Angle expectedGunAngle{ 211.1 };
    unit.setUnitRotation(HexVertexNumber(4));

    EXPECT_EQ(unit.getUnitVertexRotation(), expectedVertex);
    EXPECT_EQ(unit.getGunRotation().angle, expectedGunAngle.angle);
}

TEST_F(UnitFixture, unitGunRotationCounterclockwise)
{
    unit.setUnitRotation(HexVertexNumber(3));
    unit.setGunRotation(Angle{ 10.3 });
    HexVertexNumber expectedVertex{ 1 };
    Angle expectedGunAngle{ 250.3 };
    unit.setUnitRotation(HexVertexNumber(1));

    EXPECT_EQ(unit.getUnitVertexRotation(), expectedVertex);
    EXPECT_EQ(unit.getGunRotation().angle, expectedGunAngle.angle);
}

TEST_F(UnitFixture, unitGunRotationCounterMaxRotation)
{
    unit.setUnitRotation(HexVertexNumber(0));
    unit.setGunRotation(Angle{ 300 });
    HexVertexNumber expectedVertex{ 5 };
    Angle expectedGunAngle{ 240 };
    unit.setUnitRotation(HexVertexNumber(5));

    EXPECT_EQ(unit.getUnitVertexRotation(), expectedVertex);
    EXPECT_EQ(unit.getGunRotation().angle, expectedGunAngle.angle);
}

TEST_F(UnitFixture, unitGunRotation3Vertices)
{
    unit.setUnitRotation(HexVertexNumber(2));
    unit.setGunRotation(Angle{ 329 });
    HexVertexNumber expectedVertex{ 5 };
    Angle expectedGunAngle{ 149 };
    unit.setUnitRotation(HexVertexNumber(5));

    EXPECT_EQ(unit.getUnitVertexRotation(), expectedVertex);
    EXPECT_EQ(unit.getGunRotation().angle, expectedGunAngle.angle);
}


///////////////////////////////////////////////////////////
//////// NEXT TESTS FOR TANK DAMAGE SYSTEM ///////////////
/////////////////////////////////////////////////////////

TEST_F(UnitFixture, TankEngineDestroyed)
{
    bool expectedActionsLeft = true;
    bool expectedMoveAction = false;
    TileDistance expectedMoveLeft = { 0 };
    Shots expectedShotleft = { 3 };
    unit.applyDamage("EngineDestroyed");

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

TEST_F(UnitFixture, TankTransmissionDestroyed)
{
    bool expectedActionsLeft = true;
    bool expectedMoveAction = false;
    TileDistance expectedMoveLeft = { 0 };
    Shots expectedShotleft = { 2 };
    GameTile expectedPosition = GameTile(0, 1);
    unit.applyDamage("TransmissionDestroyed");
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 1);
    unit.moveTo(cmdMove.get(), testableBoard);

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
    EXPECT_EQ(unit.getPosition(), expectedPosition);
}


TEST_F(UnitFixture, TankCrewKilledOneByOne)
{
    bool expectedActionsLeft = false;
    bool expectedMoveAction = false;
    TileDistance expectedMoveLeft = { 0 };
    Shots expectedShotleft = { 0 };
    GameTile expectedPosition = GameTile(0, 0);
    unit.applyDamage(tankDamageSystem::kCommanderKilled);
    unit.applyDamage(tankDamageSystem::kDriverKilled);
    unit.applyDamage(tankDamageSystem::kDriverKilled);
    unit.applyDamage(tankDamageSystem::kGunnerKilled);
    unit.applyDamage(tankDamageSystem::kRadiomanKilled);
    unit.applyDamage(tankDamageSystem::kRadiomanKilled);
    unit.applyDamage(tankDamageSystem::kRadiomanKilled);
    unit.applyDamage(tankDamageSystem::kLoaderKilled);
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 1);
    unit.moveTo(cmdMove.get(), testableBoard);

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
    EXPECT_EQ(unit.getPosition(), expectedPosition);
}

// driver killed so unit cant move this turn
TEST_F(UnitFixture, TankGunCrewKilled)
{
    bool expectedShootLeft = true;
    bool expectedMoveAction = false;
    TileDistance expectedMoveLeft = { 0 }; 
    Shots expectedShotleft = { 1 };
    //unit.applyDamage(tank_state_system::kCommanderKilled);
    unit.applyDamage(tankDamageSystem::kDriverKilled);
    unit.applyDamage(tankDamageSystem::kDriverKilled);
    unit.applyDamage(tankDamageSystem::kGunnerKilled);
    unit.applyDamage(tankDamageSystem::kLoaderKilled);
    //.applyDamage(tank_state_system::kLO);

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedShootLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedShootLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}


TEST_F(UnitFixture, TankGunCrewHalfKilled)
{
    bool expectedShootLeft = true;
    bool expectedMoveAction = false;
    TileDistance expectedMoveLeft = { 0 };
    Shots expectedShotleft = { 1 };
    unit.applyDamage(tankDamageSystem::kDriverKilled);
    unit.applyDamage(tankDamageSystem::kDriverKilled);
    unit.applyDamage(tankDamageSystem::kLoaderKilled);

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedShootLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedShootLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

TEST_F(UnitFixture, TankGun1fKilled)
{
    bool expectedShootLeft = true;
    bool expectedMoveAction = true;
    TileDistance expectedMoveLeft = { 5 };
    Shots expectedShotleft = { 2 };
    unit.applyDamage(tankDamageSystem::kLoaderKilled);
    unit.applyDamage(tankDamageSystem::kLoaderKilled);

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedShootLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedShootLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

// one gyu left so tank missing one turn and then can do either move or shoot
TEST_F(UnitFixture, TankOneGuyLeftOnlyMove)
{
    bool expectedActionsLeft = false;
    bool expectedMoveAction = true;
    TileDistance expectedMoveLeft = { 4 };
    Shots expectedShotleft = { 0 };
    unit.applyDamage(tankDamageSystem::kCommanderKilled);
    unit.applyDamage(tankDamageSystem::kDriverKilled);
    unit.applyDamage(tankDamageSystem::kGunnerKilled);
    unit.applyDamage(tankDamageSystem::kRadiomanKilled);
    unit.nextTurn();
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 1);
    unit.moveTo(cmdMove.get(), testableBoard);

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), true);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

// TODO not sure if we should allow to shot in this situation
TEST_F(UnitFixture, TankImmomobileForOneTurn)
{
    bool expectedActionsLeft = true;
    bool expectedMoveAction = false;
    TileDistance expectedMoveLeft = { 0 };
    Shots expectedShotleft = { 1 };
    unit.applyDamage(tankDamageSystem::kCommanderKilled);
    unit.applyDamage(tankDamageSystem::kDriverKilled);
    unit.applyDamage(tankDamageSystem::kGunnerKilled);
    unit.applyDamage(tankDamageSystem::kRadiomanKilled);
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 1);
    unit.moveTo(cmdMove.get(), testableBoard);

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), true);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

TEST_F(UnitFixture, TankImmomobileForCurrentTurn)
{
    bool expectedActionsLeft = false;
    bool expectedMoveAction = false;
    TileDistance expectedMoveLeft = { 0 };
    Shots expectedShotleft = { 0 };
    unit.applyDamage(tankDamageSystem::kCrewShellShocked);
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 1);
    unit.moveTo(cmdMove.get(), testableBoard);

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

// unit used some activity already so damage effect will be saved for nex turn
TEST_F(UnitFixture, TankImmomobileForNextTurn)
{
    bool expectedActionsLeft = false;
    bool expectedMoveAction = false;
    TileDistance expectedMoveLeft = { 0 };
    Shots expectedShotleft = { 0 };
    auto cmdMove{ std::make_unique<MoveToAction>() };
    cmdMove->m_unitID = { unit.getID() };
    cmdMove->m_playerID = PlayerIdentifier{ 1 };
    cmdMove->m_destination = GameTile(0, 1);
    unit.moveTo(cmdMove.get(), testableBoard);
    unit.applyDamage(tankDamageSystem::kCrewShellShocked);
    unit.nextTurn();

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

TEST_F(UnitFixture, TankCrewKilled)
{
    bool expectedActionsLeft = false;
    bool expectedMoveAction = false;
    TileDistance expectedMoveLeft = { 0 };
    Shots expectedShotleft = { 0 };
    unit.applyDamage(tankDamageSystem::kCrewKilled);

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
    EXPECT_EQ(unit.isAlive(PointOfView::Enemy), true); // enemy cannot see 
    EXPECT_EQ(unit.isAlive(PointOfView::Player), false);
}

TEST_F(UnitFixture, TankExploded)
{
    bool expectedActionsLeft = false;
    bool expectedMoveAction = false;
    TileDistance expectedMoveLeft = { 0 };
    Shots expectedShotleft = { 0 };
    unit.applyDamage(tankDamageSystem::kExploded);

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
    EXPECT_EQ(unit.isAlive(PointOfView::Enemy), false); // enemy cannot see 
    EXPECT_EQ(unit.isAlive(PointOfView::Player), false);
}

TEST_F(UnitFixture, TankGunDestroyed)
{
    bool expectedShotsLeft = false;
    bool expectedMoveAction = true;
    TileDistance expectedMoveLeft = { 5 };
    Shots expectedShotleft = { 0 };
    unit.applyDamage(tankDamageSystem::kGunDestroyed);

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedShotsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedMoveAction);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
    EXPECT_EQ(unit.isAlive(PointOfView::Enemy), true);  
    EXPECT_EQ(unit.isAlive(PointOfView::Player), true);
}

TEST_F(UnitFixture, TankTurretJammed)
{
    bool expectedShotsLeft = true;
    bool expectedMoveAction = true;
    TileDistance expectedMoveLeft = { 5 };
    Shots expectedShotleft = { 3 };
    Angle angleBeforeJammed{ 270.3f };
    unit.setGunRotation(angleBeforeJammed);
    unit.applyDamage(tankDamageSystem::kTurretJammed);

    Angle angle{ 281.3f };
    unit.setGunRotation(angle);
    // angle will point to closest vertex of a hex
    Angle expectedAngle{ 270.f };
    EXPECT_EQ(unit.getGunRotation().angle, expectedAngle.angle);

    EXPECT_EQ(unit.canMove(), expectedMoveAction);
    EXPECT_EQ(unit.canShoot(), expectedShotsLeft);
    EXPECT_EQ(unit.hasActionLeft(), expectedMoveAction);
    EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
    EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}


