#include "game_message.h"

#include "damage_calculator.h"
#include "game_board.h"

core::UnitStateMsg core::GameMessageFactory::createUnitStateMsg(const GameBoard& board, const Unit* selectedUnit, const Unit* targetUnit, const DamageCalculator& calculator, const Angle& requiredGunAngleToShot, PointOfView pointOfView)
{
    UnitStateMsg resultMsg;
    resultMsg.m_id = targetUnit->getID();
    resultMsg.m_crewInfo = targetUnit->getCrewInfo();
    resultMsg.m_unitParts = targetUnit->getUnitPartsInfo();
    resultMsg.m_isAlive = targetUnit->isAlive(pointOfView);
    resultMsg.m_pointOfView = pointOfView;
    if (selectedUnit->getID() && pointOfView == PointOfView::Enemy)
    {
        Angle prevRor = selectedUnit->getGunRotation();
        calculator.fillDamageProbabilities(resultMsg.m_unitParts, selectedUnit, targetUnit, requiredGunAngleToShot);
        resultMsg.m_isRotatableTo = selectedUnit->canGunRotate(requiredGunAngleToShot);
        resultMsg.m_isInLineOfSight = selectedUnit->isTargetInLineOfSight(board, targetUnit->getPosition());
    }

    for (auto& part : resultMsg.m_unitParts)
        if (!part.m_isVisibleForEnemy && pointOfView == PointOfView::Enemy) part.m_state = DamageStatus::Hidden;
    for (auto& member : resultMsg.m_crewInfo)
        if (!member.m_isVisibleForEnemy && pointOfView == PointOfView::Enemy) member.m_state = DamageStatus::Hidden;


    return resultMsg;
}
