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

namespace details {
struct ExtendedUnitUinfo 
{

};
}
class UnitInfoTooltip : public sf::Drawable, public sf::Transformable
{
public:

    UnitInfoTooltip(const TextureHolder& textureHolder);
    void initText();
    void  show();
    void  hide();
    void  setText(const std::string& text);
    // insane amount of parameters better refactor!
    void  setContent(bool isAlive, const std::vector<std::string>& unitInfo, const std::vector<textures::ID>& iconsID, const sf::Vector2f& parentCoords);
    void  draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Text             m_unitTypeInfo;
    sf::Text             m_aliveInfo;
    sf::Font             m_font;
    sf::Color            m_aliveColor{ sf::Color::Green };
    //sf::FloatRect        m_parentBounds;

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

class GameTooltipCommon
{
public:
    GameTooltipCommon();
    void setText(const std::string& text, const sf::Vector2f& position);
    void setTextPosition(const sf::Vector2f& position) { m_text.setPosition(position); }
    void setTextColor(const sf::Color& color);
    void setContent(const std::vector<textures::ID>& iconsID, const std::string& text, const sf::Vector2f& parentCoords);
    float getTextWidth() { return m_text.getGlobalBounds().width; }
    void setCharacterSize(const int characterSize) { m_text.setCharacterSize(characterSize); }
    ~GameTooltipCommon() = default;
protected:
    sf::Text m_text;
    sf::Sprite m_icon{};
    sf::Font   m_font;
};

// actually can get rid of bunch of methods if just inherit text tooltip and override update method
class AnimatedTooltip : public ISceneNode, public GameTooltipCommon
{
public:
    AnimatedTooltip(const TextureHolder& textureHolder, float animationTime);
    void updateCurrent(sf::Time dt) override;
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getBoundingRect() const override { return  sf::FloatRect(); }
    bool isMarkedToDelete()  const override { return m_isAnimationDone; }
private:
    const TextureHolder& m_textures;
    float m_movementOffset{ kAnimatedTooltipMovementOffset };
    sf::Time m_animationTime;
    sf::Time m_currAnumationTime{ sf::seconds(0) };
    bool m_isAnimationDone{ false };
};

class TextTooltip : public ISceneNode, public GameTooltipCommon
{
public:
    TextTooltip(const std::string& text);
    void updateCurrent(sf::Time dt) override;
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::FloatRect getBoundingRect() const override { return  sf::FloatRect(); }
    bool isMarkedToDelete()  const override { return m_isRemoved; }
private:
    bool m_isRemoved{ false };
};

class BlinkingTooltip : public TextTooltip
{
public:
    enum class State {
        Idle,
        Blinking
    };
    BlinkingTooltip(const std::string& text, sf::Color color) : TextTooltip(text), m_color(color) {}
    BlinkingTooltip(const std::string& text, const sf::Time& updateTime, sf::Color color) : TextTooltip(text),
        m_updateTime(updateTime), m_color(color) {}
    void updateCurrent(sf::Time dt) override;
    void setBlinkState(const State state) { m_state = state; }
private:
    sf::Time m_updateTime{ sf::seconds(0.0f) };
    State    m_state{ State::Idle };
    sf::Color m_color;
    sf::Color m_blinkColor{ sf::Color(192, 192, 192, 200) };//sf::Color(255, 0, 0, 150) };
};
}