#pragma once


#include "rounded_rectangle_shape.h"
#include "ui_element_interface.h"

#include <SFML/Graphics/Text.hpp>
#include <functional>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Sprite.hpp>


namespace graphics 
{
class Button {
public:
    using CallbackType = std::function<void()>;
    Button(CallbackType callback, sf::Keyboard::Key hotkey = sf::Keyboard::Unknown) :
        m_callback(std::move(callback)), m_hotkey(hotkey) { }
protected:
    CallbackType m_callback;
    sf::Keyboard::Key m_hotkey;
};

class RoundedButton : public UIElement, public Button
{
public:
    RoundedButton(const sf::Vector2f& size, CallbackType callback, sf::Keyboard::Key hotkey = sf::Keyboard::Unknown);
    void  drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateCurrent(sf::Time dt) override;
    void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) override;
    sf::FloatRect getBoundingRect() const override { return getWorldTransform().transformRect(m_button.getGlobalBounds()); }
    sf::Vector2f getSize() const override { return { m_button.getGlobalBounds().width, m_button.getGlobalBounds().height }; }
    bool isMarkedToDelete()  const override { return false; }
    void setText(const std::string& text);
    void setText(const std::string_view text);
    void setTextColor(const sf::Color& textColor) { m_text.setFillColor(textColor); }
    void setCharSize(const int characterSize) { m_text.setCharacterSize(characterSize); }
    void setColor(const sf::Color& color) { m_button.setFillColor(color); }
private:
    sf::Vector2f getTextCenterPos() const;
private:
    sf::RoundedRectangleShape   m_button;
    sf::Text     m_text;
    sf::Font m_font;
};

class TexturedButton : public UIElement, public Button
{
public:
    TexturedButton(const sf::Texture& texture, CallbackType callback, sf::Keyboard::Key hotkey = sf::Keyboard::Unknown);
    void  drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateCurrent(sf::Time dt) override {}
    void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) override;
    sf::FloatRect getBoundingRect() const override { return getWorldTransform().transformRect(m_texture.getGlobalBounds()); }
    sf::Vector2f getSize() const override { return { m_texture.getGlobalBounds().width, m_texture.getGlobalBounds().height }; }
    bool isMarkedToDelete()  const override { return false; }
private:
    sf::Sprite m_texture;
};
}