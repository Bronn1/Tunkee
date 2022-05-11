#pragma once

#include "data_types.h"

#include <unordered_map>
namespace core 
{

struct ActionHistory {
    UnitIdentifier id{ 0 };
    LastActions actions{};
};

/**
 * @brief Holds actions units made in last turn. 'Last turn' is considered as a previous turn 
   if unit didn't make any action yet in current or as a current turn if unit already made something.
   Also holds history of all actions(could be usefull for players)
*/
class UnitLogger
{
public:
    void initUnits(const std::vector< UnitIdentifier>& unitIds);
    void addAction(const UnitIdentifier& id, const UnitMoveStatus);
    void cancelAction(const UnitIdentifier& id);
    LastActions getLastActions(const UnitIdentifier& id);
    void endOfTurn();
private:
    std::unordered_map<UnitIdentifier, LastActions> m_unitActions{};
    std::vector< ActionHistory> m_history{};
};
} //namespace core 
