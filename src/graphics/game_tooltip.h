#pragma once

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include <string>


constexpr int kBackgroundColorR = 169;
constexpr int kBackgroundColorG = 169;
constexpr int kBackgroundColorB = 169;
constexpr int kBackgroundColorAlpha = 120;

namespace graphics
{
    class GameTooltip : public sf::RectangleShape
    {
    public:

        GameTooltip(sf::Texture&  tooltipStates);

        void  show();
        void  hide();
        void  setText(const std::string& text);
        const sf::Text& getTextToRender() const;
        void  setParentBounds(const sf::FloatRect& parentBounds);
        void  editTooltipSizeByText();
      // void  draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::Text             m_text;
        sf::Font             m_font;
        sf::FloatRect        m_parentBounds;
        sf::Texture&         m_tooltipStates;
        sf::Sprite           m_sprite;
        const sf::Color      m_backgroundColor{ kBackgroundColorR, kBackgroundColorG, kBackgroundColorB, kBackgroundColorAlpha };
    };
}