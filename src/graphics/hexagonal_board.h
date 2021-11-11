#pragma once

#include "hexagonal_tile.h"
#include "scene_node.h"


namespace graphics {
	/**
    * Creates view of Hexagonal Board with tile selector.
	* TODO make constructor private to force everyone work through builder
    */
	class GameBoardView
	{
	public:
		GameBoardView(const std::vector<core::GameTile>& tileCoordinateSystem, int width);
		explicit GameBoardView(const int map_size);
		//bool addNewUnit(SceneNodePtr unit, core::GameTile position);
		void draw(sf::RenderWindow& target);
		const core::GameTile& getTileSelectorCoordinates() const { return m_tileSelectorCoordinates;  }
		const sf::Vector2f& getTileSelectorPosition() const { return m_tileSelector.getPosition(); }
	private:
		void initTileSelector() noexcept;

		std::vector<graphics::HexagonalTile>   m_tiles;
		sf::CircleShape  m_tileSelector;
		// TODO move to separate class tile selector
		core::GameTile   m_tileSelectorCoordinates{0, 0};
		sf::Texture      m_grassTexture;
	};
}

