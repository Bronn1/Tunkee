#pragma once


#include <string_view>
#include <vector>

namespace core
{
    constexpr std::string_view kShotMissed = "Missed";
    using MemberCount = int;
    using DamageTo = std::string_view;
    using CrewMemberName = std::string_view;

    enum class DamageStatus
    {
        Damaged,
        Normal,
        Ideal,
        Hidden 
    };

    enum class UnitActions {
        None,
        Move,
        Shot
    };

    struct UnitPartInfo {
        std::string_view m_name;
        DamageStatus m_state{ DamageStatus::Normal };
        float chanceToHitPercent{ 0.f };
        bool m_isVisibleForEnemy{ false };
    };

    using UnitPartsInfoVec = std::vector<UnitPartInfo>;

    struct CrewMemberInfo {
        CrewMemberInfo() = default;
        // TODO ger rid of bool in contsructor, change to enum for better readability
        CrewMemberInfo(std::string_view name, bool isGunMember, bool isVisibleForEnemy) : m_name(name), m_isGunMember(isGunMember), m_isVisibleForEnemy(isVisibleForEnemy) {}
        std::string_view m_name;
        DamageStatus m_state{ DamageStatus::Normal };
        MemberCount amountOfmembers{ 1 };
        bool m_isGunMember{ false };
        bool m_isVisibleForEnemy{ false };
    };

    using CrewInfo = std::vector<CrewMemberInfo>;

    class Crew {
    public:
        MemberCount initMembersCount() const noexcept;
        MemberCount getAliveMembersCount() const;
        MemberCount getAliveGunMembersCount() const;
        void applyDamage(DamageTo damageType, DamageStatus status);
        void killAll();
        CrewInfo getCrewInfo() const;
        void initCrew(CrewInfo&& crewInfo) { m_members = crewInfo; }
        bool isVisibleForEnemy() const { return m_isVisibleForEnemy; }
        void setVisibleForEnemy(bool isVisibleForEnemy);
    private:
        std::vector<CrewMemberInfo> m_members{};
        bool m_isVisibleForEnemy{ false };
    };

    class UnitPart
    {
    public:
        using enum DamageStatus;
        UnitPart(bool isVisibleForEnemy = false) : m_isVisibleForEnemy(isVisibleForEnemy) {}
        void applyDamage() { m_state = DamageStatus::Damaged; }
        DamageStatus getState() const { return m_state; }
        void setState(DamageStatus state) { m_state = state; }
        bool isVisibleForEnemy() const { return m_isVisibleForEnemy; }
        void setVisibilityForEnemy(bool isVisible = true) { m_isVisibleForEnemy = isVisible; }
    private:
        bool m_isVisibleForEnemy{ false };
        DamageStatus m_state{ DamageStatus::Normal };
    };

    //template<typename T>
    //concept UnitStateSystem = requires(T type)
    //{
     //   type.applyDamage(std::declval< const std::string_view>);
    //};
    //template<typename T> requires UnitStateSystem<T>

    enum class DamageSystemStrategies {
        TankDamageSystem
    };
}