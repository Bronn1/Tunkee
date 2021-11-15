#pragma once

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

#include <string>

namespace graphics
{
	constexpr int kBackgroundColorR = 169;
	constexpr int kBackgroundColorG = 169;
	constexpr int kBackgroundColorB = 169;
	constexpr int kBackgroundColorAlpha = 120;
	constexpr int kTooltipCharacterSize = 120;
	constexpr int kBorderOffset = 20;


	class GameTooltip : public sf::RectangleShape
	{
	public:

		GameTooltip();

		void  show();
		void  hide();
		void  setText(const std::string& text);
		const sf::Text& getTextToRender() const;
		void  setParentBounds(const sf::FloatRect& parentBounds);
		void  editTooltipSizeByText();

	private:
		sf::Text             m_text;
		sf::Font             m_font;
		sf::FloatRect        m_parentBounds;
		const sf::Color      m_backgroundColor{ kBackgroundColorR, kBackgroundColorG, kBackgroundColorB, kBackgroundColorAlpha };
	};
}