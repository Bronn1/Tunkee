#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <string_view>
#include <stdexcept>

namespace graphics
{
    /** finds new point beetwen start and end points on the board
        * \param startPoint
        * \param endPoint
        * \param factor - how far from the start should be point(0...1)
        */
    sf::Vector2f getPointBetween(const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, float distancePercent);

    sf::Vector2f getBackDirectionOffset(const float objectRotation, const float offset);
}