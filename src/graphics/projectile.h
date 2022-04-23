#pragma once

#include "interface_unit_view.h"
#include "explosion_effects.h"
#include "resource_holder.h"
#include "graphics_data_types.h"
//#include "unit_setup_view.h"

#include <SFML/Graphics/Sprite.hpp>

namespace graphics {
    class ISceneNode;
    class TankView;

    class Projectile : public ISceneNode
    {
    public:
        Projectile(const sf::Vector2f& sourcePoint, const sf::Vector2f& target, ISceneNode* parent, const  TextureHolder& textures);

        void setTarget(const sf::Vector2f& target) { m_target = target; }
        inline bool isMarkedToDelete() const override{  return m_isExploded; }
        void setScale(const sf::Vector2f& factors);
        void setDamageType(std::string_view type) { m_targetDamage = type; }
        void setRotation(const float rotation) { m_sprite.setRotation(rotation + textureRotation);  }
        void setTargetUnit(IUnitView* targetUnit) { m_targetUnit = targetUnit; }
        sf::FloatRect getBoundingRect() const { return getWorldTransform().transformRect(m_sprite.getGlobalBounds()); }
        
        void  drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        sf::Vector2f interpolateNextPoint();
        void updateCurrent(sf::Time dt) override;

        sf::Sprite m_sprite;
        sf::Vector2f m_target;
        sf::Vector2f m_sourcePoint;
        IUnitView* m_targetUnit{ nullptr };
        std::string_view m_targetDamage{};
        GraphicMovementFrame m_moveFrame;
        float textureRotation{ 90.f };
        bool m_isExploded{ false };
        const  TextureHolder& m_textures;
    };
}