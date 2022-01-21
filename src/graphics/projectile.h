#pragma once

#include "scene_node.h"
#include "explosion_effects.h"
#include "resource_holder.h"
#include "graphics_data_types.h"

#include <SFML/Graphics/Sprite.hpp>

namespace graphics {
	class SceneNode;
	class UnitView;

	class Projectile : public SceneNode
	{
	public:
		Projectile(const sf::Vector2f& sourcePoint, const sf::Vector2f& target, SceneNode* parent, const sf::Texture& texture);

		float getMaxSpeed() const { return 300.f; }
		void setTarget(const sf::Vector2f& target) { m_target = target; }
		inline bool isDestoyed() const override{  return m_isExploded; }
		void setScale(const sf::Vector2f& factors);
		void setDamageType(std::string_view type) { m_targetDamage = type; }
		void setRotation(const float rotation) { m_sprite.setRotation(rotation + textureRotation);  }
		void setTargetUnit(SceneNode* targetUnit) { m_targetUnit = targetUnit; }
		void updateCurrent(sf::Time dt) override;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		sf::Vector2f interpolateNextPoint();

		sf::Sprite m_sprite;
		sf::Vector2f m_target;
		sf::Vector2f m_sourcePoint;
		SceneNode* m_targetUnit{ nullptr };
		std::string_view m_targetDamage{};
		GraphicMovementFrame m_moveFrame;
		float textureRotation{ 90.f };
		bool m_isExploded{ false };
	};
}