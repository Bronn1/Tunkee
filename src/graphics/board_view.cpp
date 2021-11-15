#include "board_view.h"

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
            hexagon.setCoordinates(tileCoordinateSystem[x * width + y]);
            m_tiles.push_back(hexagon);

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
            m_tiles.push_back(hexagon);
        }
    }

    initTileSelector();
}

//auto is_tile_exists = [&position](TileView tile) { return  tile.getCoordinates()  == position; };
//if (auto result = std::ranges::find_if(m_tiles, is_tile_exists); result != m_tiles.end())

const sf::Vector2f& graphics::BoardView::getPositionByTileCoordinates(const core::GameTile& coordinates) const
{
    for (const auto& tile : m_tiles)
    {
        if (tile.getCoordinates() == coordinates)
            return tile.getPosition();
    }
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
    for (auto& tile: m_tiles)
    {
        target.draw(tile);
        auto mousePos = sf::Mouse::getPosition(target); // Mouse position relative to the window
        auto translatedMousePos = target.mapPixelToCoords(mousePos); // Mouse position translated into world coordinates
        if (tile.getGlobalBounds().contains(translatedMousePos)) // Rectangle-contains-point check
        {
            auto [posx, posy] = tile.getPosition();
            m_tileSelector.setPosition(posx, posy);
            m_tileSelectorCoordinates = core::GameTile(tile.getCoordinates());
           // std::cout << posx << " " << posy << "\n";
           //std::cout << tile.getCoordinates() << "\n";
        }
       
    }
    target.draw(m_tileSelector);  
}
