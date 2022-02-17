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
#include <unordered_map>

namespace graphics
{
	// TODO work a bit on unitVIew interface and sceneNode (make same as core::unit interface)
	// TODO add animation when unit shoots with jammed turrent
	 // TODO add animation when unit move slightly with unrotatable turret to shoot
	// TODO refactor behaviour  when damage recieved
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

		virtual Angle  calculateGunRotation(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint) const;
		void setGunRotation(const Angle& gunRotation);
		void  rotateTo(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint) override;
		void  markAsSelected();
		void  showTooltip(const sf::Vector2f& mouse_pos);
		inline void setTooltipText(const std::string& text) { m_tooltipDescription.setText(text); }
		void showDamage(std::string_view damageType) override;
		virtual SceneNodePtr shot(SceneNode* target, std::string_view damageType);
		void resetUnitState( std::string_view damageType);
		void setPositionBeforeMovement(const sf::Vector2f& position) { m_posBeforeMovement = position; }
		void setMovementState(std::stack < sf::Vector2f> path);
		void setCurrentRotationPoint(const sf::Vector2f& point) { m_currentRotationPoint = point; }

		virtual ~UnitView() = default;
		//UnitIdentifier getNodeByCoordinatesIfExists(const sf::Vector2f& clickedPos) const override final;
	private:
		void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		void updateCurrent(sf::Time dt) override;
		void updateMovement(sf::Time dt);


	private:
		Type m_type;
		sf::Sprite	m_bodySprite;
		sf::Sprite m_turretSprite;
		// later on after full animation support add maybe in array and remove after animation is done
		Animation  m_explosion;
		Animation  m_buriningAnimation;
		// TODO change all damage type to Unit parts 
		bool m_isDestroyed{ false };
		bool m_isSelected{ false };
		GameTooltip m_tooltipDescription;
		TextureHolder& m_textures;
		GraphicMovementFrame m_moveFrame;

		std::stack<sf::Vector2f> m_movementPath{};
		sf::Vector2f m_posBeforeMovement{ 0.f, 0.f };
		sf::Vector2f m_currentRotationPoint{ 0.f, 0.f };
		
	};

	class NullUnitView : public UnitView
	{
	public:
		NullUnitView(TextureHolder& textures) : m_emptyTexture(), UnitView(UnitIdentifier{ 0 }, Type::Basic, textures) {  }
	private:
		void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override {}
		Angle  calculateGunRotation(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint) const  override { return Angle{ 0 }; }
		void  rotateTo(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint) override {}
		SceneNodePtr shot(SceneNode* target, std::string_view damageType) override { return std::make_unique<Projectile>(getPosition(), getPosition(), this, m_emptyTexture); }

		sf::Texture m_emptyTexture;
	};
}
