#pragma once

#include "hexagonal_tile.h"
#include "../core/game_tile.h"

namespace graphics {
	/**
    * Creates view of Hexagonal Board with tile selector.
    * Basically world view of game
	* TODO make constructor private to force everyone work through builder
    */
	class HexagonalBoard
	{
	public:
		HexagonalBoard(const std::vector<core::GameTile>& tileCoordinateSystem, int width);
		explicit     HexagonalBoard(const int map_size);
		void         draw(sf::RenderWindow& target);
	private:
		void         initTileSelector() noexcept;

		std::vector<graphics::HexagonalTile>   m_hexBoard;
		sf::CircleShape                        m_tileSelector;
		sf::Texture                            m_grassTexture;
	};
}

