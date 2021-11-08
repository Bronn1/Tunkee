#include "hexagonal_tile.h"

graphics::HexagonalTile::HexagonalTile(float radius, int pointCount, int outlineThicknes) : sf::CircleShape(radius, pointCount)
{
    this->setOutlineColor(sf::Color::Black);
    this->setFillColor(sf::Color::White);
    this->setOutlineThickness(outlineThicknes);
    this->setOrigin(radius, radius);
}
