#include "board_view.h"
#include "graphics_data_types.h"
#include<iostream>
#include<ranges>

constexpr float kSide = 55;
constexpr float kOffset = 50;
constexpr int   kPointCount = 6;
constexpr float kHexOffsetMultiplierY = 0.93f;
constexpr float kHexOffsetMultiplierX = 0.75f;

namespace ranges = std::ranges;

graphics::BoardView::BoardView(const std::vector<core::GameTile>& tileCoordinateSystem, const  TextureHolder& tileTextures, int width)
{
    assert(width * width == std::size(tileCoordinateSystem));

    graphics::TileView hexagon(kSide, 0.f);


    for (int x : std::views::iota(0, width))
    {
        for (int y : std::views::iota(0, width))
        {
            hexagon.setPosition((y % 2 ? (kOffset + kSide) * kHexOffsetMultiplierY : kOffset) + x * (hexagon.getGlobalBounds().width), 
                                kOffset + y * (hexagon.getGlobalBounds().height * kHexOffsetMultiplierX));
            hexagon.setTexture(&tileTextures.get(textures::ID::TileGrass));
            hexagon.setCoordinates(tileCoordinateSystem[y * width + x]);
            m_tiles.insert({ tileCoordinateSystem[y * width + x] , hexagon});
        }
    }

    initTileSelector();
}

graphics::BoardView::BoardView(const int mapSize, const  sf::Texture& basicTexture)
{
    graphics::TileView hexagon(kSide, 0.f);
  
    for (int x : std::views::iota(0, mapSize))
    {
        for (int y : std::views::iota(0, mapSize))
        {
            hexagon.setPosition((y % 2 ? (kOffset + kSide) * kHexOffsetMultiplierY : kOffset) + x * (hexagon.getGlobalBounds().width), 
                                kOffset + y * (hexagon.getGlobalBounds().height * kHexOffsetMultiplierX));
            hexagon.setTexture(&basicTexture);
           // hexagon.setCoordinates(tileCoordinateSystem[x * mapSize + y]);
            //m_tiles.insert({ tileCoordinateSystem[x * mapSize + y] , hexagon });
        }
    }

    initTileSelector();
}

//auto is_tile_exists = [&position](TileView tile) { return  tile.getCoordinates()  == position; };
//if (auto result = std::ranges::find_if(m_tiles, is_tile_exists); result != m_tiles.end())

sf::Vector2f graphics::BoardView::getPositionByTileCoordinates(const core::GameTile& coordinates) const
{
    if (m_tiles.contains(coordinates))
    {
      // std::cout << m_tiles.at(coordinates).getPoint(1) << "\n";
       return m_tiles.at(coordinates).getPosition();
    }
}

std::stack<sf::Vector2f> graphics::BoardView::convertTileCoordinatesToScreenPos(const std::vector<core::GameTile>& coordinates) const
{
    std::stack<sf::Vector2f> positions{};
    for (auto it = std::rbegin(coordinates); it != std::rend(coordinates); it++)
    {
        auto position = getPositionByTileCoordinates(*it);
        //if(it == std::rbegin(coordinates))
        //    positions.push(position);
        positions.push(position);
    }

    return positions;
}

std::optional<core::GameTile> graphics::BoardView::getCoordinatesIfValid(const sf::Vector2f& pos) const
{
    for (auto& [coordinates, tile] : m_tiles)
        if (tile.getGlobalBounds().contains(pos))
            return { tile.getCoordinates() };

    return std::nullopt;
}

std::optional<sf::Vector2f> graphics::BoardView::getTileCenterIfValid(const sf::Vector2f& pos) const
{
    for (auto& [coordinates, tile] : m_tiles)
        if (tile.getGlobalBounds().contains(pos))
            return { tile.getPosition() };

    return std::nullopt;
}

void graphics::BoardView::selectSetupArea(const std::vector<core::GameTile>& area)
{
    sf::Color colorSetupLayer(kBoardFirstLayerColorR, kBoardFirstLayerColorG, kBoardFirstLayerColorB);
    m_setupArea.reserve(size(area));
    for (const auto& tile : area) {
        if (!m_tiles.contains(tile)) continue;

        m_setupArea.push_back(tile);
        m_tiles.at(tile).setFillColor(colorSetupLayer);
    }
}

void graphics::BoardView::resetSetupArea()
{
    for (const auto& tile : m_setupArea) {
        m_tiles.at(tile).setFillColor(sf::Color::White);
    }
    m_setupArea.clear();
}


void graphics::BoardView::resetMoveArea(std::vector<core::GameTile> moveArea, int firstLayerSize)
{
    int counter = 0;
    sf::Color colorFirstLayer(kBoardFirstLayerColorR, kBoardFirstLayerColorG, kBoardFirstLayerColorB);
    sf::Color colorSecondLayer(kBoardSecondLayerColorR, kBoardSecondLayerColorG, kBoardSecondLayerColorB, kBackSecondColorAlpha);
    for (const auto& area : m_moveArea)
    {
        m_tiles.at(area).setFillColor(sf::Color::White);
    }

    for (const auto& area : moveArea)
    {
        if (counter < firstLayerSize)
        {
            m_tiles.at(area).setFillColor(colorFirstLayer);
        }
        else
        {
            m_tiles.at(area).setFillColor(colorSecondLayer);
        }
        counter++;
    }
    m_moveArea = std::move(moveArea);
}

void graphics::BoardView::initTileSelector() noexcept
{
    m_tileSelector.setRadius(kSide);
    m_tileSelector.setPointCount(kPointCount);
    m_tileSelector.setFillColor(sf::Color::Transparent);
    m_tileSelector.setOutlineThickness(1);
    m_tileSelector.setOutlineColor(sf::Color::White);
    m_tileSelector.setOrigin(kSide, kSide);
}

void graphics::BoardView::draw(sf::RenderWindow& target)
{
    for (auto& [coordinates, tile]: m_tiles)
    {
        target.draw(tile);
        auto mousePos = sf::Mouse::getPosition(target); // Mouse position relative to the window
        auto translatedMousePos = target.mapPixelToCoords(mousePos); // Mouse position translated into world coordinates
        if (tile.getGlobalBounds().contains(translatedMousePos)) // Rectangle-contains-point check
        {
            auto [posx, posy] = tile.getPosition();
            m_tileSelector.setPosition(posx, posy);
            m_tileSelectorCoordinates = core::GameTile(tile.getCoordinates());
           //std::cout << tile.getCoordinates() << "\n";
        }
       
    }
    target.draw(m_tileSelector);  
}
