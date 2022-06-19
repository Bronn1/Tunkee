#pragma once

#include "tile_view.h"
#include "scene_node.h"
#include "resource_holder.h"

#include <unordered_map>
#include <optional>
#include <stack>


namespace graphics {
    /**
    * Creates view of Hexagonal Board with tile selector.
    * TODO make constructor private to force everyone work through builder
    */
    class BoardView
    {
    public:
        BoardView(const std::vector<core::GameTile>& tileCoordinateSystem, const  TextureHolder& tileTextures, int width);
        explicit BoardView(const int map_size, const  sf::Texture& basicTexture);
        void draw(sf::RenderWindow& target);
        core::GameTile getSelectorTileCoordinates() const { return m_tileSelectorCoordinates;  }
        sf::Vector2f getTileSelectorPosition() const { return m_tileSelector.getPosition(); }
        sf::Vector2f getPositionByTileCoordinates(const core::GameTile& coordinates) const;
        std::stack<sf::Vector2f> convertTileCoordinatesToScreenPos(const std::vector < core::GameTile>& coordinates) const;
        std::optional<core::GameTile> getCoordinatesIfValid(const sf::Vector2f& pos) const;
        std::optional<sf::Vector2f>  getTileCenterIfValid(const sf::Vector2f& pos) const;
        void selectSetupArea(const std::vector<core::GameTile>& area);
        void resetSetupArea();
        sf::Vector2f getTileVertex(const core::GameTile& coordinates, HexVertexNumber vertex) const { return m_tiles.at(coordinates).getPointGlobal(vertex.vertexNum); }

        void resetMoveArea(std::vector<core::GameTile> moveArea, int firstLayerSize);
    private:
        void initTileSelector() noexcept;

        //std::vector<graphics::TileView>   m_tiles;
        std::unordered_map<core::GameTile, graphics::TileView> m_tiles{};
        sf::CircleShape  m_tileSelector;
        core::GameTile   m_tileSelectorCoordinates{0, 0};
        std::vector<core::GameTile> m_moveArea{};
        std::vector<core::GameTile> m_setupArea{};
    };
}

