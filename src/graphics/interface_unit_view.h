#pragma once

#include "scene_node.h"
#include "src/core/unit_damage_system_strategy.h"

#include <stack>

namespace graphics
{
    /*TODO make interface more friendly to understanding
    * refactor shot function 
    */
    /***/
    class IUnitView : public ISceneNode
    {
    public:
        IUnitView(UnitIdentifier id) : m_id(id) {}
        /* destroyed units should not  vanish from map */
        bool isMarkedToDelete()  const override final { return false; }
        virtual void  markAsSelected() = 0;
        virtual void  showTooltip(const sf::Vector2f& mouse_pos) = 0;
        virtual void showDamage(std::string_view damageType) = 0;
        virtual Angle  calculateGunRotation(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint) const = 0;
        virtual void  shot(IUnitView* target, std::string_view damageType) = 0;
        /*make documetation for unitINfo format or move to separate data sctructure*/
        virtual void setTooltipMsg(bool isAlive, const  std::vector<std::string>& unitInfo, const std::vector<tankDamageSystem::DamageTo>& damageToVec) = 0;
        virtual void setUpGunRotationAnimation(const Angle& gunRotation) = 0;
        virtual void  setUpRotationAnimation(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint) = 0;
        virtual void recoveryDamage(std::string_view damageType) = 0;
        virtual void setMovementPath(std::stack < sf::Vector2f> path) = 0;
        /*
        * 
        */
        virtual void setCurrentRotationPoint(const sf::Vector2f& point) = 0;

        UnitIdentifier  getId() const { return m_id; }
        bool isPerformingAction()  const { return m_isPerformingAction; }
        void setPerformingAction(bool isPerformingAction) { m_isPerformingAction = isPerformingAction; }

    private:
        UnitIdentifier m_id{ 0 };
        bool m_isPerformingAction{ false };
    };
}

