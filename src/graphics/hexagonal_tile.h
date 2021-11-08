#pragma once

#include "../core/game_tile.h"

#include <SFML/Graphics.hpp>

namespace graphics {
	class HexagonalTile : public sf::CircleShape
	{
	public:
		explicit HexagonalTile(float radius, int pointCount, int outlineThicknes);
		const core::GameTile&     getCoordinates() const { return m_coordinates; }
		void                      setCoordinates(const core::GameTile coordinates) { m_coordinates = coordinates; }
	private:
		core::GameTile m_coordinates;
	};
}

