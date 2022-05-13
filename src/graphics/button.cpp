#include "button.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

constexpr float ktextHorizontalOrientation = 360;
constexpr int kDefaultCharSize = 15;
constexpr float kRadius = 6.f;
const sf::Color kDefaultBackgroundColor{ 0, 33, 105, 210 };
const sf::Color kDefaultBackgroundColorSelected{ 0, 4, 128 };

namespace graphics
{ 
RoundedButton::RoundedButton(const sf::Vector2f& size, CallbackType callback, sf::Keyboard::Key hotkey) :
    m_button(size, kRadius), Button(std::move(callback), hotkey)
{
    // TODO move all paths and fonts to texture manager
    m_font.loadFromFile("data/examples/comic.ttf");
    m_text.setFont(m_font);
    m_text.setRotation(ktextHorizontalOrientation);
    m_text.setCharacterSize(kDefaultCharSize);
    m_text.setString("");

    m_button.setOutlineColor(sf::Color::Black);
    m_button.setFillColor(kDefaultBackgroundColor);
    m_button.setOutlineThickness(5);
}

void RoundedButton::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_isVisible) {
        return;
    }

    target.draw(m_button, states);
    target.draw(m_text, states);
}

void RoundedButton::updateCurrent(sf::Time dt)
{
}

void RoundedButton::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos)
{
    if (!m_isVisible) {
        return;
    }
    bool isMouseInside = false;
    if (getBoundingRect().contains(mousePos)) {
        isMouseInside = true;
        m_button.setFillColor(kDefaultBackgroundColorSelected);
    }
    else {
        m_button.setFillColor(kDefaultBackgroundColor);
    }
    
    if (event.type == sf::Event::KeyPressed) {
        if (sf::Keyboard::isKeyPressed(m_hotkey)) {
            //m_hotkey.Unknown
            m_callback();
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left &&
               isMouseInside) {
            m_callback();
    }
}

void RoundedButton::setText(const std::string& text)
{
    m_text.setString(text);

    m_text.setPosition(getTextCenterPos());
}

void RoundedButton::setText(const std::string_view text)
{
    m_text.setString(std::string(text));

    m_text.setPosition(getTextCenterPos());
}

sf::Vector2f RoundedButton::getTextCenterPos() const
{
    auto borderOffsetX = (m_button.getSize().x - m_text.getGlobalBounds().width) / 2;
    auto borderOffsetY = (m_button.getSize().y - m_text.getGlobalBounds().height) / 2;

    return { m_button.getOrigin().x + borderOffsetX , m_button.getOrigin().y + borderOffsetY };
}

TexturedButton::TexturedButton(const sf::Texture& texture, CallbackType callback, sf::Keyboard::Key hotkey) :
    Button(callback, hotkey)
{
    m_texture.setTexture(texture);
    //m_texture.setColor(sf::Color::White);
}

void TexturedButton::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_isVisible) {
        return;
    }

    target.draw(m_texture, states);
}

void TexturedButton::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos)
{
    if (!m_isVisible) {
        return;
    }
    bool isMouseInside = false;
    if (getBoundingRect().contains(mousePos)) {
        isMouseInside = true;
        m_texture.setColor(kDefaultBackgroundColorSelected);
    } else {
        m_texture.setColor(kDefaultBackgroundColor);
    }

    if (event.type == sf::Event::KeyPressed) {
        if (sf::Keyboard::isKeyPressed(m_hotkey)) {
            //m_hotkey.Unknown
            m_callback();
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left &&
               isMouseInside) {
        m_callback();
    }
}

} // namespace graphics