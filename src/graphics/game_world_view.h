#pragma once

#include "scene_node.h"
#include "hexagonal_board.h"
#include "../src/core/data_types.h"

#include <map>
#include <memory>

using SceneNodePtr = std::unique_ptr<graphics::SceneNode>;
using BoardViewPtr = std::unique_ptr<graphics::GameBoardView>;

namespace graphics {

	class GameWorldView
	{
	public:

		GameWorldView(sf::RenderWindow& target, BoardViewPtr board);
		bool addNewUnitView(SceneNodePtr unit, core::GameTile position);
		void draw();
		void update(sf::Event& event);
	private:
		sf::RenderWindow& m_renderTarget;
		BoardViewPtr m_board;
		std::map<ObjectIdentifier, SceneNodePtr, Comparator<ObjectIdentifier>>  m_unitsGraph{};
		PlayerIdentifier m_playerId{ 0 };
		ObjectIdentifier  m_selectedUnitId{ 0 };


	};
}