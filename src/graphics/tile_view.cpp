#include "tile_view.h"

constexpr int sidesAmount = 6;

graphics::TileView::TileView(float radius, float outlineThicknes) : sf::CircleShape(radius, sidesAmount)
{
    setOutlineColor(sf::Color::Black);
    setFillColor(sf::Color::White);
    setOutlineThickness(outlineThicknes);
    setOrigin(radius, radius);
}