#include "game_tooltip.h"

#include <SFML/Graphics/RenderTarget.hpp>
constexpr int kTooltipCharacterSize = 15;
constexpr float kBorderOffset = 5.f;
constexpr float kInterlineOffset = 8.f;
constexpr float ktextHorizontalOrientation = 360;

#include <iostream>

graphics::UnitInfoTooltip::UnitInfoTooltip(const TextureHolder& textureHolder) : m_textures(textureHolder)
{
    // TODO move all fonts to texture manager
    m_font.loadFromFile("data/examples/comic.ttf");
    initText();
    m_background.setOutlineColor(sf::Color::Black);
    m_background.setFillColor(m_backgroundColor);
    m_background.setOutlineThickness(1);
    m_movementStatusIcon.setScale({ 0.5f, 0.5f });
    m_movementStatusIcon.setTexture(m_textures.get(textures::ID::MovementInfoIcon));
    m_shootingStatusIcon.setScale({ 0.5f, 0.5f });
    m_shootingStatusIcon.setTexture(m_textures.get(textures::ID::ShootingInfoIcon));
}

void graphics::UnitInfoTooltip::initText()
{
    m_unitTypeInfo.setFont(m_font);
    m_unitTypeInfo.setRotation(ktextHorizontalOrientation);
    m_unitTypeInfo.setCharacterSize(kTooltipCharacterSize);
    m_unitTypeInfo.setFillColor(sf::Color::Black);
    m_aliveInfo.setFont(m_font);
    m_aliveInfo.setRotation(ktextHorizontalOrientation);
    m_aliveInfo.setCharacterSize(kTooltipCharacterSize);
    m_aliveInfo.setFillColor(sf::Color::Green);
    m_movementInfo.setFont(m_font);
    m_movementInfo.setRotation(ktextHorizontalOrientation);
    m_movementInfo.setCharacterSize(kTooltipCharacterSize);
    m_movementInfo.setFillColor(sf::Color::Black);
    m_shootingInfo.setFont(m_font);
    m_shootingInfo.setRotation(ktextHorizontalOrientation);
    m_shootingInfo.setCharacterSize(kTooltipCharacterSize);
    m_shootingInfo.setFillColor(sf::Color::Black);
 
    m_hiddenDamageCounter.setFont(m_font);
    m_hiddenDamageCounter.setRotation(ktextHorizontalOrientation);
    m_hiddenDamageCounter.setCharacterSize(kTooltipCharacterSize);
    m_hiddenDamageCounter.setFillColor(sf::Color::Black);
}

void graphics::UnitInfoTooltip::show()
{
    m_unitTypeInfo.setFillColor(sf::Color::Black);
    m_aliveInfo.setFillColor(m_aliveColor);
    m_movementInfo.setFillColor(sf::Color::Black);
    m_shootingInfo.setFillColor(sf::Color::Black);
    m_hiddenDamageCounter.setFillColor(sf::Color::Black);
    m_background.setFillColor(m_backgroundColor);
    m_background.setOutlineThickness(1);
    for (auto& icon : m_icons) icon.setColor(sf::Color::White);
    m_shootingStatusIcon.setColor(sf::Color::White);
    m_movementStatusIcon.setColor(sf::Color::White);
}

void graphics::UnitInfoTooltip::hide()
{
    m_unitTypeInfo.setFillColor(sf::Color::Transparent);
    m_aliveInfo.setFillColor(sf::Color::Transparent);
    m_background.setFillColor(sf::Color::Transparent);
    m_movementInfo.setFillColor(sf::Color::Transparent);
    m_shootingInfo.setFillColor(sf::Color::Transparent);
    m_hiddenDamageCounter.setFillColor(sf::Color::Transparent);
    m_background.setOutlineThickness(0);
    for (auto& icon : m_icons) icon.setColor(sf::Color::Transparent);
    m_shootingStatusIcon.setColor(sf::Color::Transparent);
    m_movementStatusIcon.setColor(sf::Color::Transparent);
}

void graphics::UnitInfoTooltip::setText(const std::string& text)
{
}

void graphics::UnitInfoTooltip::setContent(bool isAlive, const  std::vector<std::string>& unitInfo, const std::vector<textures::ID>& iconsID, const sf::Vector2f& parentCoords)
{
    m_icons.clear();
    float prevX = kBorderOffset;
    float prevY = kBorderOffset;
    float maxTextWidth = 0.f;
    // a lot of code here but basically it just calculates pos of previous item in tooltip and place next
    // needs refactor a bit
    m_unitTypeInfo.setString("Tank, Basic tank ");
    maxTextWidth = m_unitTypeInfo.getGlobalBounds().width;
    m_unitTypeInfo.setPosition(parentCoords.x + prevX, parentCoords.y + prevY);
    prevY += m_unitTypeInfo.getGlobalBounds().height + kInterlineOffset;

    m_aliveColor = isAlive ? sf::Color::Green : sf::Color::Red;
    m_aliveInfo.setString(isAlive ? "Alive": "Destroyed");
    maxTextWidth = std::max(maxTextWidth, m_aliveInfo.getGlobalBounds().width);
    m_aliveInfo.setPosition(parentCoords.x + prevX, parentCoords.y + prevY);
    prevY += m_aliveInfo.getGlobalBounds().height + kInterlineOffset;

    m_movementStatusIcon.setPosition(parentCoords.x + prevX, parentCoords.y + prevY);
    float xTmpPos = m_movementStatusIcon.getGlobalBounds().width + kBorderOffset;
    m_movementInfo.setString(unitInfo.at(0)+ "(" + unitInfo.at(3) + ")" + "; ");
    m_movementInfo.setPosition(parentCoords.x + prevX + xTmpPos, parentCoords.y + prevY);
    xTmpPos += m_movementInfo.getGlobalBounds().width + kBorderOffset;

    m_shootingStatusIcon.setPosition(parentCoords.x + prevX + xTmpPos, parentCoords.y + prevY);
    xTmpPos += m_shootingStatusIcon.getGlobalBounds().width + kBorderOffset;
    m_shootingInfo.setString(unitInfo.at(1));
    m_shootingInfo.setPosition(parentCoords.x + prevX + xTmpPos, parentCoords.y + prevY);
    xTmpPos += m_shootingInfo.getGlobalBounds().width + kInterlineOffset;

    maxTextWidth = std::max(maxTextWidth, xTmpPos);
    prevY += std::max(m_shootingInfo.getGlobalBounds().height, m_movementStatusIcon.getGlobalBounds().height) + kInterlineOffset;
    maxTextWidth = std::max(maxTextWidth, m_aliveInfo.getGlobalBounds().width);
    float iconHeight = 0.f;
    float lastIconOffset = 0.f;
    float maxIconsWidth = 0.f;
    float maxIconsHeight = 0.f;
    for (const auto& id : iconsID)
    {
        

        m_icons.push_back(sf::Sprite());
        m_icons.back().setScale({ 0.4f, 0.4f });
        m_icons.back().setTexture(m_textures.get(id));
        m_icons.back().setPosition(parentCoords.x + prevX, parentCoords.y + prevY);
        
        maxIconsWidth += m_icons.back().getGlobalBounds().width + m_icons.back().getGlobalBounds().width / 2.f;
        iconHeight = m_icons.back().getGlobalBounds().height;
        if (id == textures::ID::HiddenDamageIcon)
        {
            m_hiddenDamageCounter.setString("x" + unitInfo.at(2));
            m_hiddenDamageCounter.setPosition(parentCoords.x + prevX + m_icons.back().getGlobalBounds().width + kBorderOffset, 
                                              parentCoords.y + prevY + iconHeight/ 2.f);
            prevX += m_hiddenDamageCounter.getGlobalBounds().width + kBorderOffset;
            maxIconsWidth += m_hiddenDamageCounter.getGlobalBounds().width + kBorderOffset;
        } 
        prevX += m_icons.back().getGlobalBounds().width + m_icons.back().getGlobalBounds().width / 2.f;
        lastIconOffset  = m_icons.back().getGlobalBounds().width / 2.f;  
    }
    prevX -= lastIconOffset + kBorderOffset;
    maxIconsHeight = iconHeight + prevY + kBorderOffset;
    // TODO REFACTOR( or at least make constant for it) '??/??' means unknown so only enemy has unknown action state -> change color to red
    m_backgroundColor = (unitInfo.at(1) != "??/??") ? sf::Color(kGreenColorR, kGreenColorG, kGreenColorB, kGreenColorAlpha) :
                                                      sf::Color(kRedColorR, kRedColorG, kRedColorB, kRedColorAlpha);
    m_background.setSize({ std::max(maxIconsWidth, maxTextWidth), maxIconsHeight });
    m_background.setPosition(parentCoords);
}

void graphics::UnitInfoTooltip::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_background, states);
    for (const auto& icon : m_icons)
        target.draw(icon, states);
    target.draw(m_unitTypeInfo, states);
    target.draw(m_aliveInfo, states);
    target.draw(m_movementStatusIcon, states);
    target.draw(m_shootingStatusIcon, states);
    target.draw(m_movementInfo, states);
    target.draw(m_shootingInfo, states);
    target.draw(m_hiddenDamageCounter, states);
}

graphics::GameTooltipCommon::GameTooltipCommon()
{
    //TODO remove hardcode, move to resource holder
    if (!m_font.loadFromFile("data/examples/comic.ttf")) {
        throw std::runtime_error("BLEATTTTTTTTTTTT\n");
    }
}

graphics::AnimatedTooltip::AnimatedTooltip(const TextureHolder& textureHolder, float animationTime) : 
    m_textures(textureHolder), m_animationTime(sf::seconds(animationTime))
{
    m_text.setFont(m_font);
    m_text.setRotation(ktextHorizontalOrientation);
    m_text.setCharacterSize(kTooltipCharacterSize);
    m_text.setFillColor(sf::Color::Transparent);
    m_text.setString("");
    m_icon.setColor(sf::Color::Transparent);
    m_icon.setScale({ 0.4f, 0.4f });
}

void graphics::AnimatedTooltip::updateCurrent(sf::Time dt)
{
    m_currAnumationTime += dt;
    if (m_currAnumationTime > m_animationTime)
    {
        m_isAnimationDone = true;
        m_icon.setColor(sf::Color::Transparent);
        m_text.setFillColor(sf::Color::Transparent);
    }
    m_icon.move({ 0, -m_movementOffset });
    m_text.move({ 0, -m_movementOffset });
}

void graphics::AnimatedTooltip::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_text, states);
    target.draw(m_icon, states);
}

void graphics::GameTooltipCommon::setText(const std::string& text, const sf::Vector2f& position)
{
    m_text.setString(text);
    m_text.setPosition(position.x, position.y);
}

void graphics::GameTooltipCommon::setTextColor(const sf::Color& color)
{
    m_text.setFillColor(color);
}

void graphics::GameTooltipCommon::setContent(const std::vector<textures::ID>& iconsID, const std::string& text, const sf::Vector2f& parentCoords)
{}

graphics::TextTooltip::TextTooltip(const std::string& text)
{
    m_text.setFont(m_font);
    m_text.setString(text);
}

void graphics::TextTooltip::updateCurrent(sf::Time dt)
{
}

void graphics::TextTooltip::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_text, states);
}

void graphics::BlinkingTooltip::updateCurrent(sf::Time dt)
{
    // for now just hardcode color
    m_updateTime += dt;
    if (m_state == State::Idle) return;
    if (m_updateTime > sf::seconds(0.7f)) {
        m_text.setFillColor((m_text.getColor() == m_blinkColor) ? m_color : m_blinkColor);
        m_updateTime = sf::seconds(0);
    }
}
