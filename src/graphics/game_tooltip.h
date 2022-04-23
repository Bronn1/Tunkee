#pragma once

#include "resource_holder.h"
#include "scene_node.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <string>
#include <unordered_set>

namespace graphics
{
    // TODO move graphics constant to one place in separate namespace 
    constexpr int kGreenColorR = 229;
    constexpr int kGreenColorG = 255;
    constexpr int kGreenColorB = 204;
    constexpr int kGreenColorAlpha = 100;

    constexpr int kRedColorR = 228;
    constexpr int kRedColorG = 78;
    constexpr int kRedColorB = 78;
    constexpr int kRedColorAlpha = 100;
    constexpr float kAnimatedTooltipMovementOffset = 1.f;

    class GameTooltip : public sf::Drawable, public sf::Transformable
    {
    public:

        GameTooltip(const TextureHolder& textureHolder);
        void initText();
        void  show();
        void  hide();
        void  setText(const std::string& text);
        // insane amount of parameters better refactor!
        void  setContent(bool isAlive,  const std::vector<std::string>& unitInfo, const std::vector<textures::ID>& iconsID, const sf::Vector2f& parentCoords);
        void  draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::Text             m_unitTypeInfo;
        sf::Text             m_aliveInfo;
        sf::Font             m_font;
        sf::Color            m_aliveColor{ sf::Color::Green };
        sf::FloatRect        m_parentBounds;

        sf::Sprite           m_movementStatusIcon;
        sf::Sprite           m_shootingStatusIcon;
        sf::Text             m_movementInfo;
        sf::Text             m_shootingInfo;
        sf::Text             m_hiddenDamageCounter;

        std::vector<sf::Sprite> m_icons{};
        sf::RectangleShape   m_background;
        const TextureHolder& m_textures;
        sf::Color      m_backgroundColor{ kGreenColorR, kGreenColorG, kGreenColorB, kGreenColorAlpha };
    };

    class AnimatedTooltip : public ISceneNode
    {
    public:

        AnimatedTooltip(const TextureHolder& textureHolder, float animationTime);
        void setContent(const std::vector<textures::ID>& iconsID, const std::string& text, const sf::Vector2f& parentCoords);
        void setText(const std::string& text, const sf::Vector2f& parentCoords);
        void updateCurrent(sf::Time dt) override;
        void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        sf::FloatRect getBoundingRect() const override  { return  sf::FloatRect(); }
        bool isMarkedToDelete()  const override { return m_isAnimationDone; }
    private:
        sf::Text m_movingText;
        sf::Sprite m_movingIcon{};
        const TextureHolder& m_textures;
        float m_movementOffset{ kAnimatedTooltipMovementOffset };
        sf::Time m_animationTime;
        sf::Time m_currAnumationTime{ sf::seconds(0) };
        bool m_isAnimationDone{ false };
        sf::Font    m_font;
    };
}