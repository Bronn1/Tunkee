#include "board_view.h"
#include "graphics_data_types.h"
#include<iostream>
#include<ranges>

constexpr float kSide = 55;
constexpr float kOffset = 50;
constexpr int   kPointCount = 6;
constexpr float kHexOffsetMultiplierY = 0.93f;
constexpr float kHexOffsetMultiplierX = 0.75f;


graphics::BoardView::BoardView(const std::vector<core::GameTile>& tileCoordinateSystem, sf::Texture* basicTexture, int width)
{
    assert(width * width == std::size(tileCoordinateSystem));

    graphics::TileView hexagon(kSide, 0.f);


    for (int x : std::views::iota(0, width))
    {
        for (int y : std::views::iota(0, width))
        {
            hexagon.setPosition((y % 2 ? (kOffset + kSide) * kHexOffsetMultiplierY : kOffset) + x * (hexagon.getGlobalBounds().width), 
                                kOffset + y * (hexagon.getGlobalBounds().height * kHexOffsetMultiplierX));
            hexagon.setTexture(basicTexture);
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
       return m_tiles.at(coordinates).getPosition();
    }
}

std::vector<sf::Vector2f> graphics::BoardView::getVectorPositionsByTiles(const std::vector<core::GameTile>& coordinates) const
{
    std::vector<sf::Vector2f> positions{};
    for (const auto& tile : coordinates)
    {
        auto position = getPositionByTileCoordinates(tile);
        positions.push_back(position);
    }

    return positions;
}

std::optional<core::GameTile> graphics::BoardView::getCoordinatesIfValid(const sf::Vector2f& pos) const
{
    for (auto& [coordinates, tile] : m_tiles)
    {
        if (tile.getGlobalBounds().contains(pos))
        {
            return { tile.getCoordinates() };
        }
    }

    return std::nullopt;
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
