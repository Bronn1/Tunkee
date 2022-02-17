#include "unit.h"
#include "pathfinding.h"

#include <ranges>
#include <cmath>

namespace views = std::views;
namespace ranges = std::ranges;

inline bool core::Unit::canMove() const
{
    if (m_damageSystemStrategy->amountOfActionCanDo() <= 1 && m_actionState == ActionState::ShotPerformed)
        return false;

    return (getRemainingMovement() > TileDistance{ 0 } && m_damageSystemStrategy->isMovingSystemsAlive());
}

inline bool core::Unit::canShoot() const
{
    if (m_damageSystemStrategy->amountOfActionCanDo() <= 1 && m_actionState == ActionState::MovePerformed)
        return false;

    return (getRemainingShots() > Shots{ 0 } && m_damageSystemStrategy->isGunsAlive());
}

void core::Unit::defaultApplyDamage(const std::string_view damageType)
{
    auto actionStatus = isUnitHaveFullActionState();
    m_damageSystemStrategy->applyDamage(damageType, actionStatus);
    if (actionStatus == ActionStatus::full)
        setActionState(actionStatus); // applaying new amount of move pts and shots after damage
}

std::vector<core::GameTile> core::Unit::defaultMoveTo(const MoveToAction* moveToCmd, GameBoard& board)
{
    rotateTo(moveToCmd->m_destination, board);

    if (!canMove()) return std::vector<GameTile>{};

    auto pathToDest = pathfinding::getShortestPath(board, m_position, board.getTile(moveToCmd->m_destination));
    auto adjustedPathVec = adjustPathByAvailableMovement(pathToDest);
    unsigned pathSize = std::size(adjustedPathVec);
    if (pathSize == 0) return adjustedPathVec;

    changeStateByMovement(TileDistance{ pathSize });
    board.setTileAccessible(m_position, true);
    m_position = adjustedPathVec.back();
    board.setTileAccessible(m_position, false);

    return adjustedPathVec;
}

void core::Unit::defaultRotateTo(const GameTile& destination, GameBoard& board)
{
    auto vertex = getUnitVertexRotation();
    HexVertexNumber curVertex{ vertex.vertexNum }; // left vertex to cur
    int distance = pathfinding::getDistance(m_position, destination);
    const int kBypassAllHexVertices = 4;
    for (const int i : views::iota(0, kBypassAllHexVertices))
    {
        auto tilesInfrontVec = pathfinding::getLineOfSightWithoutObstacles(board, m_position, TileDistance{ (unsigned)distance + 5 }, HexVertexNumber{ m_unitRotation.vertexNum + i });
        if (ranges::find(tilesInfrontVec, destination) != end(tilesInfrontVec))
        {
            m_rotationCounter += i;
            rotateToVertex(HexVertexNumber{ m_unitRotation.vertexNum + i });
            return;
        }
        tilesInfrontVec = pathfinding::getLineOfSightWithoutObstacles(board, m_position, TileDistance{ (unsigned)distance + 5 }, HexVertexNumber{ m_unitRotation.vertexNum - i });
        if (ranges::find(tilesInfrontVec, destination) != end(tilesInfrontVec))
        {
            m_rotationCounter += i;
            rotateToVertex(HexVertexNumber{ m_unitRotation.vertexNum + i });
            return;
        }
    }

    // TODO could be good to do refactor here, cuz we shouldn't be there ,unit should be able to find his proper rotation
    // if we have  reached this part something is very wrong!
    // throw std::runtime_error("pzd\n");
}

bool core::Unit::defaultShoot(const Shots& shots)
{
    if (!canShoot())
        return false;

    changeStateByShooting(shots);
    return true;
}

void core::Unit::defaultNextTurn()
{
    m_damageSystemStrategy->nextTurn();
    setActionState(ActionStatus::full);
    m_rotationCounter = 0;
}

void core::Unit::defaultSetUnitRotation(const HexVertexNumber rotation)
{
    // we need to rotate gun(turret) relatively to new parent rotatin
    int rotatationDiff = m_unitRotation.vertexNum - rotation.vertexNum;
    m_unitRotation = rotation;
    // 60 degree angle between every vertex, so count vertex diff and then multiply
    float gunAdder = -rotatationDiff * 60;
    m_gunRotation = Angle{ m_gunRotation.angle + gunAdder };
}

int core::Unit::defaultGetArmor(const Angle& attackingAngle) const
{	// TODO not tested yet
    const Angle kFrontalArmorAngleFrom = Angle{ 180 - 60 };
    const Angle kFrontalArmorAngleTo = Angle{ 180 + 60 };
    Angle resultAngle = VertexToAngle(m_unitRotation) - attackingAngle;
    resultAngle.angle = int(resultAngle.angle + 0.5);

    //Angle unitRotationAngle = VertexToAngle(m_unitRotation);
    // std::cout << "attackingAngle = " << attackingAngle.angle << " \n";
    // std::cout << "attackingAngleShifted = " << attackingAngleShifted << " \n";
    // std::cout << "kFrontalArmorAngleFrom = " << kFrontalArmorAngleFrom.angle << " \n";
    // std::cout << "kFrontalArmorAngleTo = " << kFrontalArmorAngleTo.angle << " \n";
    // std::cout << "unitRotationAngle = " << unitRotationAngle.angle << " \n";
    // std::cout << "resultAngle = " << resultAngle.angle << " \n";

    if (resultAngle >= kFrontalArmorAngleFrom && resultAngle <= kFrontalArmorAngleTo)
        return m_armor.m_frontal;
    else
        return m_armor.m_side;
}

MoveAreaInfo core::Unit::defaultGetMoveArea(const GameBoard& board) const
{
    TileDistance halfMovement = getHalfMovePointsRoundedUp();
    TileDistance remainingMovement = getRemainingMovement();
    std::vector<GameTile> moveArea = {};
    int firstLayerSize = 0;

    // full Movement and half Movement should be showed with different colors in game
    if (remainingMovement >= halfMovement) {
        TileDistance remainingMovementInFirstHalf = getRemainingMoveInFirstHalf();
        moveArea = pathfinding::getAvailableAreaWithRotation(board, getPosition(), remainingMovementInFirstHalf, getUnitVertexRotation());
        firstLayerSize = static_cast<int>(std::size(moveArea));
        auto  fullMoveArea = pathfinding::getAvailableAreaWithRotation(board, getPosition(), remainingMovement, getUnitVertexRotation());
        for (auto& tile : fullMoveArea)
            // add only new(unique) tiles
            if (std::ranges::find(moveArea, tile) == std::end(moveArea))
                moveArea.push_back(tile);
    }
    else {
        moveArea = pathfinding::getAvailableAreaWithRotation(board, getPosition(), remainingMovement, getUnitVertexRotation());
        firstLayerSize = 0;
    }

    return MoveAreaInfo{ std::move(moveArea), firstLayerSize };//moveArea;
}

ActionStatus core::Unit::isUnitHaveFullActionState() const
{
    if (getRemainingMovement() == getFullMovement() && getRemainingShots() == getRateOfFire())
        return ActionStatus::full;

    return ActionStatus::empty;
}

std::vector<core::GameTile>& core::Unit::adjustPathByAvailableMovement(std::vector<GameTile>& pathToDest)
{
    unsigned costSoFar = 0;
    auto [move] = getRemainingMovement();

    auto rmv_if = [&costSoFar, &move](const GameTile& t) {costSoFar += t.travelCost(); return costSoFar > move; };
    std::erase_if(pathToDest, rmv_if);

    return pathToDest;
}

void core::Unit::rotateToVertex(const HexVertexNumber vertex)
{
    const int kDefaultMAxRotation = 2;
    auto m_prevRotation = m_unitRotation;
    setUnitRotation(vertex);
    if (m_rotationCounter < kDefaultMAxRotation || vertex == m_prevRotation) return;

    auto [move] = getRemainingMovement();
    if (move)
        changeStateByMovement(TileDistance{ 1 });
}

void core::UnitActionState::setActionState(const ActionStatus& state)
{
    if (state == ActionStatus::full) {
        m_remainingMovePoints = m_damageSystemStrategy->getMoveDistanceWithFine(m_fullMovePoints);
        m_remainingShots = m_damageSystemStrategy->getRateOfFireWithFine(m_rateOfFire);
        m_actionState = NoActionsPerformed;
    }
    else {
        m_remainingMovePoints = { 0 };
        m_remainingShots = { 0 };
        m_actionState = BothActionsPerformed;
    }
}

void core::UnitActionState::defaultChangeStateByMovement(const TileDistance& distance)
{
    if (distance > m_remainingMovePoints)
        m_remainingMovePoints = { 0 };
    else
        m_remainingMovePoints -= distance;


    TileDistance halfDistance = getHalfMovePoints();
    if (m_actionState == NoActionsPerformed || m_actionState == MovePerformed) {
        if (m_remainingMovePoints < halfDistance || m_damageSystemStrategy->amountOfActionCanDo() <= 1)
            m_remainingShots = { 0 };
        else if (m_remainingShots == m_damageSystemStrategy->getRateOfFireWithFine(m_rateOfFire))
            m_remainingShots -= getHalfShots();

        m_actionState = MovePerformed;
    }
    else if (m_actionState == ShotPerformed)
    {
        // TODO throw exception if less than zero
        m_remainingShots = getHalfShotsRoundedUp() - (m_damageSystemStrategy->getRateOfFireWithFine(m_rateOfFire) - m_remainingShots);
        m_actionState = BothActionsPerformed;
    }
}

void core::UnitActionState::defaultChangeStateByShooting(const Shots& shots)
{
    if (shots > m_remainingShots)
        m_remainingShots = Shots{ 0 };
    else
        m_remainingShots -= shots;

    Shots halfShots = getHalfShots();
    if (m_actionState == NoActionsPerformed || m_actionState == ShotPerformed) {
        if (m_remainingShots < halfShots || m_damageSystemStrategy->amountOfActionCanDo() <= 1)
            m_remainingMovePoints = { 0 };
        else if (m_remainingMovePoints == m_damageSystemStrategy->getMoveDistanceWithFine(m_fullMovePoints))
            m_remainingMovePoints -= getHalfMovePoints();

        m_actionState = ShotPerformed;
    }
    else if (m_actionState == MovePerformed)
    {
        // TODO throw exception if less than zero
        m_remainingMovePoints = getHalfMovePointsRoundedUp() - (m_damageSystemStrategy->getMoveDistanceWithFine(m_fullMovePoints) - m_remainingMovePoints);
        m_actionState = BothActionsPerformed;
    }
}

TileDistance core::UnitActionState::getRemainingMoveInFirstHalf() const
{
    TileDistance remainingInFirstAction{ m_remainingMovePoints - getHalfMovePointsRoundedUp() };
    if (remainingInFirstAction.distance == 0) return  remainingInFirstAction;

    return TileDistance((m_damageSystemStrategy->getMoveDistanceWithFine(m_fullMovePoints).distance % 2 == 0) ? (remainingInFirstAction.distance) : (remainingInFirstAction.distance + 1));
}

Shots core::UnitActionState::getHalfShotsRoundedUp() const
{
    Shots rateOfFireWithFine = m_damageSystemStrategy->getRateOfFireWithFine(m_rateOfFire);
    return Shots((rateOfFireWithFine.shots % 2 == 0) ? (rateOfFireWithFine.shots / 2) : (rateOfFireWithFine.shots / 2) + 1);
}

TileDistance core::UnitActionState::getHalfMovePointsRoundedUp() const
{
    TileDistance fullMovePtsWithFine = m_damageSystemStrategy->getMoveDistanceWithFine(m_fullMovePoints);
    return TileDistance((fullMovePtsWithFine.distance % 2 == 0) ? (fullMovePtsWithFine.distance / 2) : (fullMovePtsWithFine.distance / 2) + 1);
}

void core::UnitActionState::setDamageStrategy(DamageSystemStrategies strategy, Crew crewAmount)
{
    switch (strategy)
    {
    case DamageSystemStrategies::TankDamageSystem:
        m_damageSystemStrategy = std::make_unique<tankDamageSystem::TankDamageSystemStrategy>(crewAmount);
        break;
    default:
        m_damageSystemStrategy = std::make_unique<tankDamageSystem::TankDamageSystemStrategy>(crewAmount);
        break;
    }
}

core::TankUnit::TankUnit(UnitIdentifier id, TileDistance dis, Shots rateOfFire)
    : Unit(id, dis, rateOfFire)
{
    m_type = UnitType::Tank;

    setDamageStrategy(DamageSystemStrategies::TankDamageSystem, Crew{ 5, 3 });
    //applyDamage(tankDamageSystem::kTurretJammed);
}

void core::TankUnit::setGunRotation(const Angle& angle)
{
    if (m_damageSystemStrategy->isGunsRotatable())
        m_gunRotation = angle;
    else
    {
        // if gun took damage and became non rotatable we'll round to closest vertex
        HexVertexNumber vertex = AngleToClosestVertex(m_gunRotation);
        m_gunRotation = VertexToAngle(vertex);
    }
    std::cout << " " << m_gunRotation.angle << " model2\n";
}

bool core::TankUnit::isTargetInLineOfSight(const GameBoard& board, const GameTile& target) const
{
    int distance = pathfinding::getDistance(getPosition(), target);
    auto lineOfSight = pathfinding::getLineOfSightWithoutObstacles(board, getPosition(), TileDistance{ (unsigned)distance + 5 }, AngleToClosestVertex(getGunRotation()));

    if (auto isInlineOfSight = ranges::find(lineOfSight, target); isInlineOfSight == end(lineOfSight))
    {
        // TODO print game message in GUI
        std::cout << "Unit: " << getID().identifier << " doesn't have  target tile: " << target << " in line of sight\n";
        return false;
    }
    return true;
}

MoveAreaInfo core::TankUnit::getMoveArea(const GameBoard& board) const
{
    return defaultGetMoveArea(board);
}
// v predelax etix uglov pustb vrashaetca
// samoxodka povorchivaetsa s animation i potom obratno

