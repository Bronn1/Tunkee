#include "game_tooltip.h"

graphics::GameTooltip::GameTooltip()  
{
	// move all fonts to texture manager
	m_font.loadFromFile("data/examples/comic.ttf");
	
	m_text.setFont(m_font);
	constexpr float textHorizontalOrientation = 90;
	m_text.setRotation(textHorizontalOrientation);
	m_text.setCharacterSize(kTooltipCharacterSize);
	m_text.setColor(sf::Color::Black);

	setOutlineColor(sf::Color::Black);
	setFillColor(m_backgroundColor);
	setOutlineThickness(1);
}

void graphics::GameTooltip::show()
{
	setText("HallfffffffA\nsdfdsfsdfdsf\n11232+\n11232+\n11232+");
	m_text.setColor(sf::Color::Black);

	setFillColor(m_backgroundColor);
	setOutlineThickness(1);
}

void graphics::GameTooltip::hide()
{
	m_text.setColor(sf::Color::Transparent);
	setFillColor(sf::Color::Transparent);
	setOutlineThickness(0);
}

void graphics::GameTooltip::setText(const std::string& text)
{
	m_text.setString(text);
	editTooltipSizeByText();
}

const sf::Text& graphics::GameTooltip::getTextToRender() const 
{
	return m_text;
}

void graphics::GameTooltip::setParentBounds(const sf::FloatRect& parentBounds)
{
	m_parentBounds = parentBounds;
}

void graphics::GameTooltip::editTooltipSizeByText()
{
	sf::FloatRect boundsText = m_text.getGlobalBounds();

	setSize({ boundsText.width + kBorderOffset, boundsText.height + kBorderOffset });
	setOrigin(boundsText.width, 0);
	setPosition(m_parentBounds.width / 2.f, m_parentBounds.height / 4.f);
	auto [x1, y1] = getPosition();
	m_text.setPosition(x1 + kBorderOffset, y1 + kBorderOffset);
}
