#pragma once

#include "scene_node.h"

namespace graphics
{
	class EntityView : public SceneNode
	{
	public:
		void setVelocity(sf::Vector2f velocity);
		void setVelocity(float vx, float vy);
		sf::Vector2f getVelocity() const;

	protected:
		virtual void updateCurrent(sf::Time dt);

	private:
		sf::Vector2f m_velocity;
	};
}

