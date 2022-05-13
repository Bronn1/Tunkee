#include "unit_logger.h"

#include <ranges>
#include <algorithm>
#include <string>

namespace ranges = std::ranges;

void core::UnitLogger::initUnits(const std::vector<UnitIdentifier>& unitIds)
{
    ranges::transform(unitIds, std::inserter(m_unitActions, end(m_unitActions)), 
                    [](auto& unitId) { 
                        return  std::pair{ unitId, LastActions{} }; 
                    });
}

void core::UnitLogger::addAction(const UnitIdentifier& id, const UnitMoveStatus action)
{
    if (!m_unitActions.contains(id)) {
        return; // at setup stage unit not added yet to logger
        //throw std::runtime_error("Invalid unit ID:" + std::to_string(id.identifier) + "in logger is recieved\n"); // switch to custom exceptions
    }

    m_unitActions.at(id).moveAction = action;
    m_unitActions.at(id).activeStatus = ActiveStatus::OneAction;
    if (action == UnitMoveStatus::MovingFullSpeed) {
        m_unitActions.at(id).activeStatus = ActiveStatus::TwoActions;
    }
    
}

//void core::UnitLogger::cancelAction(const UnitIdentifier& id)
//{}
//unit is out of moves

LastActions core::UnitLogger::getLastActions(const UnitIdentifier& id)
{
    if (!m_unitActions.contains(id)) {// at setup stage unit not added yet to logger
        return { UnitMoveStatus::Idleing ,  ActiveStatus::Inactive };
       // throw std::runtime_error("Invalid unit ID:" + std::to_string(id.identifier) + "in logger is recieved\n"); // switch to custom exceptions
    }

    return m_unitActions.at(id);
}

void core::UnitLogger::endOfTurn()
{
    for (auto& [id, lastActions] : m_unitActions) {
        if (lastActions.activeStatus == ActiveStatus::Inactive) {
            lastActions = { UnitMoveStatus::Idleing, ActiveStatus::Inactive };
        }
        lastActions.activeStatus = ActiveStatus::Inactive;
    }
}
