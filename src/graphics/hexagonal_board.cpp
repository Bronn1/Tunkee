#include "hexagonal_board.h"

#include<iostream>
#include<ranges>

constexpr float kSide = 55;
constexpr float kOffset = 50;
constexpr int   kPointCount = 6;

graphics::HexagonalBoard::HexagonalBoard(const std::vector<core::GameTile>& tileCoordinateSystem, int width)
{
    assert(width * width == std::size(tileCoordinateSystem));

    graphics::HexagonalTile hexagon(kSide, 6, 0);

    // TODO fix hardcoded path
    if (!m_grassTexture.loadFromFile("data/textures/grass-test1.jpg"))
        std::cerr << "Can not load texture\n";
    m_grassTexture.setSmooth(true);

    for (int x : std::views::iota(0, width))
    {
        for (int y : std::views::iota(0, width))
        {
            hexagon.setPosition((y % 2 ? (kOffset + kSide) * 0.93f : kOffset) + x * (hexagon.getGlobalBounds().width), kOffset + y * (hexagon.getGlobalBounds().height * 0.75));
            hexagon.setTexture(&m_grassTexture);
            hexagon.setCoordinates(tileCoordinateSystem[x * width + y]);
            m_hexBoard.push_back(hexagon);

        }
    }

    initTileSelector();
}

graphics::HexagonalBoard::HexagonalBoard(const int mapSize)
{
    graphics::HexagonalTile hexagon(kSide, 6, 0);

    // TODO fix hardcoded path
    if (!m_grassTexture.loadFromFile("data/textures/grass-test1.jpg"))
        std::cerr << "Can not load texture\n";
    m_grassTexture.setSmooth(true);
  
    for (int x : std::views::iota(0, mapSize))
    {
        for (int y : std::views::iota(0, mapSize))
        {
            hexagon.setPosition((y % 2 ? (kOffset + kSide) * 0.93f : kOffset) + x * (hexagon.getGlobalBounds().width), kOffset + y * (hexagon.getGlobalBounds().height * 0.75));
            hexagon.setTexture(&m_grassTexture);
            m_hexBoard.push_back(hexagon);
        }
    }

    initTileSelector();
}

void graphics::HexagonalBoard::initTileSelector() noexcept
{
    m_tileSelector.setRadius(kSide);
    m_tileSelector.setPointCount(kPointCount);
    m_tileSelector.setFillColor(sf::Color::Transparent);
    m_tileSelector.setOutlineThickness(1);
    m_tileSelector.setOutlineColor(sf::Color::White);
    m_tileSelector.setOrigin(kSide, kSide);
}

void graphics::HexagonalBoard::draw(sf::RenderWindow& target)
{
    for (auto& tile: m_hexBoard)
    {
        target.draw(tile);
        auto mouse_pos = sf::Mouse::getPosition(target); // Mouse position relative to the window
        auto translated_pos = target.mapPixelToCoords(mouse_pos); // Mouse position translated into world coordinates
        if (tile.getGlobalBounds().contains(translated_pos)) // Rectangle-contains-point check
        {
            auto [posx, posy] = tile.getPosition();
            m_tileSelector.setPosition(posx, posy);
           // std::cout << posx << " " << posy << "\n";
            std::cout << tile.getCoordinates() << "\n";
        }
    }
    target.draw(m_tileSelector);
}
