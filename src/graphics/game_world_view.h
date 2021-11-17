#pragma once

#include "scene_node.h"
#include "board_view.h"
#include "../src/core/data_types.h"
#include "../controllers/movement_controller.h"

#include <map>
#include <memory>

using SceneNodePtr = std::unique_ptr<graphics::SceneNode>;
//using BoardViewPtr = std::unique_ptr<graphics::BoardView>;
using BoardViewPtr = graphics::BoardView;


namespace graphics {

	class GameWorldView
	{
	public:
		//GameWorldView() = default;
		GameWorldView(sf::RenderWindow& target, BoardViewPtr board, controllers::MovementController controller);
		bool addNewUnitView(SceneNodePtr unit, core::GameTile position);
		void draw();
		void update(sf::Event& event);
	private:
		bool selectNewUnit(const sf::Vector2f& mousePos);
		void moveSelectedUnit(const sf::Vector2f& mousePos);
		void showMoveAreaForUnit();
		void clearMoveArea();
	private:
		sf::RenderWindow& m_renderTarget;
		BoardViewPtr m_board;
		std::map<ObjectIdentifier, SceneNodePtr, Comparator<ObjectIdentifier>>  m_unitsGraph{};
		PlayerIdentifier m_playerId{ 0 };
		ObjectIdentifier  m_selectedUnitId{ 0 };
		controllers::MovementController m_movementConroller;
	};
}