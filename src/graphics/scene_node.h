#pragma once


#include "../core/data_types.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <optional>
#include <vector>
#include <memory>



namespace sf {
    class RenderWindow;
}

namespace graphics 
{
    
    /**
     * @brief Base class for every grahic nodeon the board
    */
    class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
    {
    public:
        using SceneNodePtr = std::unique_ptr<SceneNode>;
        SceneNode();
        void  attachChild(SceneNodePtr child);
        SceneNodePtr  detachChild(const SceneNode& node);
        void  update(sf::Time dt);
        UnitIdentifier  getId() const { return m_id; }
        virtual void  showTooltip(const sf::Vector2f& mouse_pos) {}
        virtual void  rotateTo(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint){}
        virtual sf::FloatRect getBoundingRect() const { return sf::FloatRect(); }
        virtual void  drawAsSelected() {}
        virtual inline void showDamage(std::string_view damageType){}
        bool isPerformingAction()  const { return m_isPerformingAction; }
        void setPerformAction(const bool actionState) { m_isPerformingAction = actionState; }
        virtual bool isDestoyed()  const { return false; }
        /** finds new point beetwen start and end points on the board
        * \param startPoint
        * \param endPoint
        * \param factor - how far from the start should be point(0...1)
        */
        virtual sf::Vector2f interpolateWithFactor(const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, float factor) const;

        sf::Vector2f  getWorldPosition() const;
        sf::Transform  getWorldTransform() const;
        virtual ~SceneNode() = default;
    private:
        virtual void updateCurrent(sf::Time dt);
        void updateChildren(sf::Time dt);
        virtual void  draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void  drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        void  drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

    protected:
        UnitIdentifier                 m_id{ 0 };
        SceneNode* m_parent;
        bool m_isPerformingAction{ false };
    private:
        std::vector<SceneNodePtr>		m_children;
    };
}