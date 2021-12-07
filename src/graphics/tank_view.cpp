//
// Created by ä on 06.11.2021.
//

#include "tank_view.h"

#include <numbers>
#include "SFML/Graphics/RenderTarget.hpp"

constexpr float kDefaultUnitRotation = 270.f;
constexpr float kDefaultUnitScale = 0.17f;

graphics::TankView::TankView(UnitIdentifier id, Type type, const sf::Texture& textures, const sf::Texture& turrretTexture)
	:
	m_type(type)
	, m_bodySprite(textures), m_turretSprite(turrretTexture)
{
	m_id = id;
	sf::FloatRect bounds = m_bodySprite.getLocalBounds();
	sf::FloatRect turretBounds = m_turretSprite.getLocalBounds();
	// setting up center point
	m_turretSprite.setOrigin(turretBounds.width / kTurrentCenterPointDividerX, turretBounds.height / 2.f);
	m_bodySprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	auto [x, y] = m_bodySprite.getPosition();
	m_turretSprite.setPosition(x, y);
	m_tooltipDescription.setParentBounds(bounds);

	setRotation(kDefaultUnitRotation);
	setScale(kDefaultUnitScale, kDefaultUnitScale);

	EntityView::setVelocity({ kBasicVelocity, kBasicVelocity });
}

void graphics::TankView::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_bodySprite, states);
	target.draw(m_turretSprite, states);
	target.draw(m_tooltipDescription, states);
	target.draw(m_tooltipDescription.getTextToRender(), states);
}

sf::FloatRect graphics::TankView::getBoundingRect() const
{
	return getWorldTransform().transformRect(m_bodySprite.getGlobalBounds());
}

void graphics::TankView::rotateTurretTo(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint)
{
	float dx = curPoint.x - targetPoint.x;
	float dy = curPoint.y - targetPoint.y;

	float turretTextureOffset = 90;
	float rotation = ((atan2(dy, dx)) * 180 / std::numbers::pi) + turretTextureOffset;
	m_turretSprite.setRotation(rotation);
}

void graphics::TankView::rotateTo(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint)
{
	float dx = curPoint.x - targetPoint.x;
	float dy = curPoint.y - targetPoint.y;

	float rotation = ((atan2(dy, dx)) * 180 / std::numbers::pi);
	setRotation(rotation);
	float turretTextureOffset = 0;
	m_turretSprite.setRotation(turretTextureOffset);
}

void graphics::TankView::drawAsSelected()
{
	if (m_isSelected) {
		m_bodySprite.setColor(sf::Color::White);
		m_turretSprite.setColor(sf::Color::White);
	}
	else {
		sf::Color selectionColor(224, 255, 255, 200);
		m_bodySprite.setColor(selectionColor);
		m_turretSprite.setColor(selectionColor);
	}
	m_isSelected = m_isSelected ? false : true;
}

void graphics::TankView::showTooltip(const sf::Vector2f& mouse_pos) 
{
	if (getBoundingRect().contains(mouse_pos)) {
		m_tooltipDescription.show();
	}
	else {
		m_tooltipDescription.hide();
	}
}
