#include "gtest/gtest.h"
#include "gmock/gmock.h"


#include "src/core/unit.h"
#include "src/core/game_tile.h"

using namespace core;

class UnitFixture : public ::testing::Test
{
public:
	// So unit can do per turn  either 5 move or 3 shots or 2 shots and 3 move
	TankUnit unit{ UnitIdentifier{ 1 }, TileDistance{5}, Shots{3} };
	TankUnit unitWithEvenaction{ UnitIdentifier{ 1 }, TileDistance{4}, Shots{6} };
	UnitFixture()
	{
	}

	void SetUp()
	{
		GameTile pos(0,0);
		unit.setPosition(pos);
		unitWithEvenaction.setPosition(pos);
	}


	void TearDown() {

	}

	~UnitFixture() {
	}
};

TEST_F(UnitFixture, zeroActionsAfterMoveMove)
{
	std::vector<GameTile> firstActionPath{ GameTile(0,1), GameTile(0,2), GameTile(0,3) };
	unit.moveTo(firstActionPath);
	std::vector<GameTile> secondActionPath{ GameTile(1,3), GameTile(2,3), GameTile(3,3) }; // only 2 steps left
	unit.moveTo(secondActionPath);
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
	std::vector<GameTile> firstActionPath{ GameTile(0,1), GameTile(0,2), GameTile(0,3) };
	unit.moveTo(firstActionPath);
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
	std::vector<GameTile> firstActionPath{ GameTile(0,1), GameTile(0,2) };
	unit.moveTo(firstActionPath);
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
	std::vector<GameTile> firstActionPath{ GameTile(0,1), GameTile(0,2), GameTile(0,3) };
	unit.moveTo(firstActionPath);
	Shots shot{ 1 };
	unit.shots(shot);
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
	std::vector<GameTile> firstActionPath{ GameTile(0,1) };
	unit.moveTo(firstActionPath);
	std::vector<GameTile> firstActionPath2{ GameTile(0,2) };
	unit.moveTo(firstActionPath2);
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
	std::vector<GameTile> firstActionPath{ GameTile(0,1) };
	unit.moveTo(firstActionPath);
	std::vector<GameTile> firstActionPath2{ GameTile(0,2) };
	unit.moveTo(firstActionPath2);
	unit.moveTo(firstActionPath);
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
	unit.shots(shot);
	unit.shots(shot);

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
	unit.shots(shot);
	std::vector<GameTile> firstActionPath{ GameTile(0,1), GameTile(0,2) };
	unit.moveTo(firstActionPath);

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
	std::vector<GameTile> firstActionPath{ GameTile(0,1), GameTile(0,2), GameTile(0,3) };
	unit.moveTo(firstActionPath);
	Shots shot{ 2 };
	unit.shots(shot);
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
	unit.shots(shot);
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
	unit.shots(shot);
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
	unit.shots(shot);
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
	unitWithEvenaction.shots(shot);
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

TEST_F(UnitFixture, 1MoveAfterMoveEvenAmount)
{
	std::vector<GameTile> firstActionPath{ GameTile(0,1), GameTile(0,2), GameTile(0,3) };
	unitWithEvenaction.moveTo(firstActionPath);
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

TEST_F(UnitFixture, 1actionAfterMoveEvenAmount)
{
	std::vector<GameTile> firstActionPath{ GameTile(0,1), GameTile(0,2) };
	unitWithEvenaction.moveTo(firstActionPath);
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
	std::vector<GameTile> firstActionPath{ GameTile(0,1) };
	unitWithEvenaction.moveTo(firstActionPath);
	std::vector<GameTile> firstActionPath2{ GameTile(0,2) };
	unitWithEvenaction.moveTo(firstActionPath2);
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

/////////////////////////
// NEXT TESTs FOR TANK DAMAGE SYSTEm


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
	std::vector<GameTile> firstActionPath{ GameTile(0,1) };
	unit.moveTo(firstActionPath);

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
	unit.applyDamage(tank_state_system::kCommanderKilled);
	unit.applyDamage(tank_state_system::kDriverKilled);
	unit.applyDamage(tank_state_system::kDriverKilled);
	unit.applyDamage(tank_state_system::kGunnerKilled);
	unit.applyDamage(tank_state_system::kRadiomanKilled);
	unit.applyDamage(tank_state_system::kRadiomanKilled);
	unit.applyDamage(tank_state_system::kRadiomanKilled);
	unit.applyDamage(tank_state_system::kLoaderKilled);
	std::vector<GameTile> firstActionPath{ GameTile(0,1) };
	unit.moveTo(firstActionPath);

	EXPECT_EQ(unit.canMove(), expectedMoveAction);
	EXPECT_EQ(unit.canShoot(), expectedActionsLeft);
	EXPECT_EQ(unit.hasActionLeft(), expectedActionsLeft);
	EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
	EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
	EXPECT_EQ(unit.getPosition(), expectedPosition);
}

TEST_F(UnitFixture, TankGunCrewKilled)
{
	bool expectedShootLeft = true;
	bool expectedMoveAction = true;
	TileDistance expectedMoveLeft = { 5 };
	Shots expectedShotleft = { 1 };
	//unit.applyDamage(tank_state_system::kCommanderKilled);
	unit.applyDamage(tank_state_system::kDriverKilled);
	unit.applyDamage(tank_state_system::kDriverKilled);
	unit.applyDamage(tank_state_system::kGunnerKilled);
	unit.applyDamage(tank_state_system::kLoaderKilled);
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
	bool expectedMoveAction = true;
	TileDistance expectedMoveLeft = { 5 };
	Shots expectedShotleft = { 1 };
	unit.applyDamage(tank_state_system::kDriverKilled);
	unit.applyDamage(tank_state_system::kDriverKilled);
	unit.applyDamage(tank_state_system::kLoaderKilled);

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
	unit.applyDamage(tank_state_system::kLoaderKilled);
	unit.applyDamage(tank_state_system::kLoaderKilled);

	EXPECT_EQ(unit.canMove(), expectedMoveAction);
	EXPECT_EQ(unit.canShoot(), expectedShootLeft);
	EXPECT_EQ(unit.hasActionLeft(), expectedShootLeft);
	EXPECT_EQ(unit.getRemainingMovement(), expectedMoveLeft);
	EXPECT_EQ(unit.getRemainingShots(), expectedShotleft);
}

