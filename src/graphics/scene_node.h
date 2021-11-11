#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "../core/data_types.h"
#include "../core/game_tile.h"

#include <optional>
#include <vector>
#include <memory>


namespace graphics 
{
	/** Base class for every grahic node on the board, uses composite pattern to represent
	* all dependent grapic nodes
	*/
	class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
	{
	public:
		using  SceneNodePtr = std::unique_ptr<SceneNode>;

	public:
		SceneNode();

		void  attachChild(SceneNodePtr child);
		SceneNodePtr  detachChild(const SceneNode& node);

		void  update(sf::Time dt);
		ObjectIdentifier  getId() const { return m_id; }
		virtual void  showTooltip(const sf::Vector2f& mouse_pos) {}
		//virtual ObjectIdentifier getNodeByCoordinatesIfExists(const sf::Vector2f& clickedPos) const;
		virtual void  moveTo(sf::Time dt) {}
		virtual void  rotateTurretTo(float angle) {}
		virtual sf::FloatRect getBoundingRect() const = 0;
		virtual void  setAsSelected() {}

		sf::Vector2f  getWorldPosition() const;
		sf::Transform  getWorldTransform() const;
		virtual ~SceneNode() {

		}
	private:
		virtual void updateCurrent(sf::Time dt);
		void	updateChildren(sf::Time dt);

		virtual void  draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void  drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		void  drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

	protected:
		ObjectIdentifier                m_id{ 0 };
	private:
		std::vector<SceneNodePtr>		m_children;
		SceneNode*                      m_parent;
	};
}