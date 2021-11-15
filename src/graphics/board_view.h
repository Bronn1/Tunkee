#pragma once

#include "tile_view.h"
#include "scene_node.h"


namespace graphics {
	/**
    * Creates view of Hexagonal Board with tile selector.
	* TODO make constructor private to force everyone work through builder
    */
	class BoardView
	{
	public:
		BoardView(const std::vector<core::GameTile>& tileCoordinateSystem, sf::Texture* basicTexture, int width);
		explicit BoardView(const int map_size, const  sf::Texture& basicTexture);
		void draw(sf::RenderWindow& target);
		const core::GameTile& getTileSelectorCoordinates() const { return m_tileSelectorCoordinates;  }
		const sf::Vector2f& getTileSelectorPosition() const { return m_tileSelector.getPosition(); }
		const sf::Vector2f& getPositionByTileCoordinates(const core::GameTile& coordinates) const;
	private:
		void initTileSelector() noexcept;

		std::vector<graphics::TileView>   m_tiles;
		sf::CircleShape  m_tileSelector;
		core::GameTile   m_tileSelectorCoordinates{0, 0};
	};
}

