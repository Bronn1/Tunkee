#include "game_message.h"

#include "damage_calculator.h"
#include "game_board.h"


auto moveStatus = "Idle";
// TODO maybe make sense switch to icon  or show as an animation, 
// cant figure it out yet, just for tests will be text
constexpr std::string_view kIdleMoveStatus = "Idle";
constexpr std::string_view kHalfMoveStatus = "Moving with half speed";
constexpr std::string_view kFullMoveStatus = "Moving with full speed";
constexpr std::string_view kHiddenActionInfo = "??/??";


core::GameMessageBuilder core::GameMessageBuilder::createUnitStateMsg(const Unit* targetUnit, const PointOfView pointOfView)
{
    GameMessageBuilder msgBuiler{};
    msgBuiler.m_targetUnit = targetUnit;
    // TODO we should ADD info about amount of times different damage was done including ricochet and maybe missed !
    msgBuiler.m_stateMsg.crewInfo = targetUnit->getCrewInfo();
    msgBuiler.m_stateMsg.unitParts = targetUnit->getUnitPartsInfo();
    for (auto& part : msgBuiler.m_stateMsg.unitParts)
        if (!part.m_isVisibleForEnemy && pointOfView == PointOfView::Enemy) 
            part.m_state = DamageStatus::Hidden;
    for (auto& member : msgBuiler.m_stateMsg.crewInfo)
        if (!member.m_isVisibleForEnemy && pointOfView == PointOfView::Enemy) 
            member.m_state = DamageStatus::Hidden;

    msgBuiler.m_stateMsg.id = targetUnit->getID();
    msgBuiler.m_stateMsg.isAlive = targetUnit->isAlive(pointOfView);
    msgBuiler.m_stateMsg.pointOfView = pointOfView;

    // adding text unit info in readable format
    msgBuiler.m_stateMsg.hiddenDamageCounter = targetUnit->getHiddenDamageCounter();
    if (pointOfView == PointOfView::Player) {
        msgBuiler.m_stateMsg.unitInfo.at(0) =
            std::to_string(targetUnit->getRemainingMovement().distance) + "/" + std::to_string(targetUnit->getFullMovement().distance);
        msgBuiler.m_stateMsg.unitInfo.at(1) =
            std::to_string(targetUnit->getRemainingShots().shots) + "/" + std::to_string(targetUnit->getRateOfFire().shots);
    } else {
        msgBuiler.m_stateMsg.unitInfo.at(0) = kHiddenActionInfo;
        msgBuiler.m_stateMsg.unitInfo.at(1) = kHiddenActionInfo;
    }
    msgBuiler.m_stateMsg.unitInfo.at(2) = std::to_string(targetUnit->getHiddenDamageCounter());

   
    return msgBuiler;
}

core::GameMessageBuilder& core::GameMessageBuilder::addMoveStatus(const LastActions lastActions)
{
    m_stateMsg.lastActions = lastActions;
    auto moveStatus = kIdleMoveStatus;
    // maybe switch to icons instead of text
    if (m_stateMsg.lastActions.moveAction == UnitMoveStatus::MovingFullSpeed) {
        moveStatus = kFullMoveStatus;
    } else if (m_stateMsg.lastActions.moveAction == UnitMoveStatus::MovingHalfSpeed) {
        moveStatus = kHalfMoveStatus;
    }
    m_stateMsg.unitInfo.at(3) = std::string(moveStatus);

    return *this;
}

core::GameMessageBuilder& core::GameMessageBuilder::addEnemyInfo(const GameBoard& board, const Unit* selectedUnit, const DamageCalculator& calculator, const Angle& requiredGunAngleToShot)
{
    if (selectedUnit->getID() && m_stateMsg.pointOfView == PointOfView::Enemy) {
        Angle prevRor = selectedUnit->getGunRotation();
        calculator.fillDamageProbabilities(m_stateMsg.unitParts, selectedUnit, m_targetUnit, requiredGunAngleToShot);
        m_stateMsg.isRotatableTo = selectedUnit->canGunRotate(requiredGunAngleToShot);
        m_stateMsg.isInLineOfSight = selectedUnit->isTargetInLineOfSight(board, m_targetUnit->getPosition());
    }

    return *this;
}
