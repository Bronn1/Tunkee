#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf {
    class Time;
}

namespace gui {
 class TextLabel : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
    enum class Dialogs
    {
        QuitGame,
        SinglePlayer,
        None
    };
    explicit TextLabel(const sf::Font& font, const int characterSize, const sf::Color& textColor);
    void  draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void  update(const sf::Time& dt);
    void setText(const std::string& text) { m_text.setString(text); }
    void setPosition(const sf::Vector2f position) { m_text.setPosition(position); }
    void setColor(const sf::Color& textColor) { m_text.setFillColor(textColor); }
    float getWidth() { return m_text.getGlobalBounds().width; }
private:
    sf::Text             m_text;
    sf::Font             m_font;
};
}