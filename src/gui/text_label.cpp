
#include "text_label.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

constexpr float ktextHorizontalOrientation = 360;

gui::TextLabel::TextLabel(const sf::Font& font, const int characterSize, const sf::Color& textColor)
{
    m_text.setFont(font);
    m_text.setRotation(ktextHorizontalOrientation);
    m_text.setCharacterSize(characterSize);
}

void gui::TextLabel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_text, states);
}

void gui::TextLabel::update(const sf::Time& dt)
{
}
