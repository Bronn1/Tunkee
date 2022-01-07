#pragma once

#include "entity_view.h"
#include "game_tooltip.h"
#include "explosion_effects.h"
#include "resource_holder.h"
#include "projectile.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <stack>

namespace graphics
{
	using SceneNodePtr = std::unique_ptr<SceneNode>;

	class UnitView : public EntityView
	{
	public:
		enum class Type
		{
			Basic
		};
	public:
		UnitView(UnitIdentifier id, Type type, TextureHolder& textures);
		sf::FloatRect       getBoundingRect() const;

		virtual Angle  rotateGunTo(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint);
		sf::Vector2f getGunPeakPosition();
		void  rotateTo(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint) override;
		void  markAsSelected();
		void  showTooltip(const sf::Vector2f& mouse_pos);
		inline void setTooltipText(const std::string& text) { m_tooltipDescription.setText(text); }
		inline void markAsDestoyed() override { m_isDestroyed = true; }
		virtual SceneNodePtr shot(const sf::Vector2f& destination);
		void setPositionBeforeMovement(const sf::Vector2f& position) { m_posBeforeMovement = position; }
		void setMovementState(std::stack < sf::Vector2f> path);

		virtual ~UnitView() = default;
		//UnitIdentifier getNodeByCoordinatesIfExists(const sf::Vector2f& clickedPos) const override final;
	private:
		void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		void updateCurrent(sf::Time dt) override;


	private:
		Type m_type;
		sf::Sprite	m_bodySprite;
		sf::Sprite m_turretSprite;
		Animation  m_explosion;
		Animation  m_explosion2;
		bool m_isDestroyed{ false };
		bool m_isSelected{ false };
		GameTooltip m_tooltipDescription;
		TextureHolder& m_textures;
		GraphicMovementFrame m_moveFrame;

		std::stack<sf::Vector2f> m_movementPath{};
		sf::Vector2f m_posBeforeMovement{ 0.f, 0.f };
		
	};

	class NullUnitView : public UnitView
	{
	public:
		NullUnitView(TextureHolder& textures) : m_emptyTexture(), UnitView(UnitIdentifier{ 0 }, Type::Basic, textures) {  }
	private:
		void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override {}
		Angle  rotateGunTo(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint) override { return Angle{ 0 }; }
		void  rotateTo(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint) override {}
		SceneNodePtr shot(const sf::Vector2f& destination) override { return std::make_unique<Projectile>(getPosition(), getPosition(), this, m_emptyTexture); }

		sf::Texture m_emptyTexture;
	};
}
