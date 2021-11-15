#include "tile_view.h"

constexpr int sidesAmount = 6;

graphics::TileView::TileView(float radius, float outlineThicknes) : sf::CircleShape(radius, sidesAmount)
{
    this->setOutlineColor(sf::Color::Black);
    this->setFillColor(sf::Color::White);
    this->setOutlineThickness(outlineThicknes);
    this->setOrigin(radius, radius);
}
