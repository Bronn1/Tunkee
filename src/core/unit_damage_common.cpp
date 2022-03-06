#include "unit_damage_common.h"

#include<numeric>
core::MemberCount core::Crew::initMembersCount() const noexcept
{
    return  std::accumulate(begin(m_members), end(m_members), 0,
        [](const std::size_t previous, const auto& element) { return previous + element.amountOfmembers; });
}

core::MemberCount core::Crew::getAliveMembersCount() const
{
    auto sumAliveMembers = [](const std::size_t previous, const auto& element)
    {
        int adder = element.amountOfmembers;
        if (element.m_state == DamageStatus::Damaged) adder = 0;
        return previous + adder;
    };
    return  std::accumulate(begin(m_members), end(m_members), 0, sumAliveMembers);
}

core::MemberCount core::Crew::getAliveGunMembersCount() const
{
    auto sumAliveGunMembers = [](const std::size_t previous, const auto& element)
    {
        int adder = 0;
        if (element.m_isGunMember && element.m_state != DamageStatus::Damaged)
            adder = element.amountOfmembers;
        return previous + adder;
    };
    return  std::accumulate(begin(m_members), end(m_members), 0, sumAliveGunMembers);
}

void core::Crew::applyDamage(DamageTo damageType, DamageStatus status)
{
    auto ifNameEqual = [&damageType](const auto& crewInfo) { return crewInfo.m_name == damageType; };
    auto crewIt = std::find_if(begin(m_members), end(m_members), ifNameEqual);
    if (crewIt == end(m_members))
        return;

    (*crewIt).m_state = status;
}

void core::Crew::killAll()
{
    for (auto& member : m_members)
        member.m_state = DamageStatus::Damaged;
}

core::CrewInfo core::Crew::getCrewInfo() const
{
    return m_members;
}

void core::Crew::setVisibleForEnemy(bool isVisibleForEnemy)
{
    m_isVisibleForEnemy = isVisibleForEnemy;
    for (auto& member : m_members)
        member.m_isVisibleForEnemy = isVisibleForEnemy;
}
