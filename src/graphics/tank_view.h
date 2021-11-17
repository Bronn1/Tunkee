#pragma once

#include "entity_view.h"
#include "game_tooltip.h"

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

namespace graphics
{

	constexpr int   kBasicVelocity = 40;
	constexpr float kTurrentCenterPointDividerX = 1.25;

	class TankView : public EntityView
	{
	public:
		enum Type
		{
			Eagle,
			Raptor,
		};


	public:
		TankView(ObjectIdentifier id, Type type, const sf::Texture& textures, const sf::Texture& turrretTexture);
		sf::FloatRect       getBoundingRect() const;

		void  moveTo(sf::Time dt);
		void  rotateTurretTo(float angle);
		void  drawAsSelected();
		void  showTooltip(const sf::Vector2f& mouse_pos);
		inline void setTooltipText(const std::string& text) { m_tooltipDescription.setText(text); }
		//ObjectIdentifier getNodeByCoordinatesIfExists(const sf::Vector2f& clickedPos) const override final;
	private:
		void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;


	private:
		Type m_type;
		sf::Sprite	m_bodySprite;

		sf::Sprite m_turretSprite;
		bool m_isSelected{ false };
		GameTooltip m_tooltipDescription;
		
	};
}
