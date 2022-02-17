#pragma once

#include "../core/game_tile.h"

#include <SFML/Graphics.hpp>

namespace graphics {
    class TileView : public sf::CircleShape
    {
    public:
        explicit TileView(float radius, float outlineThicknes);
        sf::Vector2f getPointGlobal(size_t index) const { return getTransform().transformPoint(getPoint(index));  }
        const core::GameTile&     getCoordinates() const { return m_coordinates; }
        void                      setCoordinates(const core::GameTile coordinates) { m_coordinates = coordinates; }
    private:
        core::GameTile m_coordinates;
    };
}

