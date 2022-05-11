
#include "text_label.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

constexpr float ktextHorizontalOrientation = 360;
constexpr float kBackgroundEdgeRadius = 6;
constexpr float kEdgeOffset = 50;
namespace gui 
{
TextLabel::TextLabel(const sf::Font& font, const int characterSize, const sf::Color& textColor)
{
    m_text.setFont(font);
    m_text.setRotation(ktextHorizontalOrientation);
    m_text.setCharacterSize(characterSize);
    m_background.setFillColor(sf::Color::Transparent);
}

void gui::TextLabel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_text, states);
    target.draw(m_background, states);
}

void gui::TextLabel::update(const sf::Time& dt)
{
}

void gui::TextLabel::setBackground(const sf::Color& color)
{
    auto floatRect = m_text.getGlobalBounds();
    m_background = sf::RoundedRectangleShape({ floatRect.width  + kEdgeOffset, floatRect.height  + kEdgeOffset }, 
                                             kBackgroundEdgeRadius);
    m_background.setFillColor(color);
    m_background.setOutlineThickness(5);
    auto borderOffsetX = (m_background.getSize().x - m_text.getGlobalBounds().width) / 2;
    auto borderOffsetY = (m_background.getSize().y - m_text.getGlobalBounds().height) / 2;
    m_background.setPosition(m_text.getPosition());
    //return { m_button.getOrigin().x + borderOffsetX , m_button.getOrigin().y + borderOffsetY };
}
} // namespace gui