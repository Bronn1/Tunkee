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
     * @brief Base class for every grahic node on the board
    */
    class ISceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
    {
    public:
        using SceneNodePtr = std::unique_ptr<ISceneNode>;
        ISceneNode() = default;
        virtual sf::FloatRect getBoundingRect() const = 0;
        virtual bool isMarkedToDelete()  const = 0;
    
    private:
        virtual void updateCurrent(sf::Time dt) = 0;
        virtual void  drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const = 0;
    public:
        void  update(sf::Time dt);
        sf::Vector2f  getWorldPosition() const;
        sf::Transform  getWorldTransform() const;
        void  attachChild(SceneNodePtr child);
        SceneNodePtr  detachChild(const ISceneNode& node);
        virtual ~ISceneNode() = default;
    private:
        void updateChildren(sf::Time dt);
        virtual void  draw(sf::RenderTarget& target, sf::RenderStates states) const;    
        void  drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
    protected:
        ISceneNode* m_parent{ nullptr };
    private:
        std::vector<SceneNodePtr>		m_children{};
    };
}