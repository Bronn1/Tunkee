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

    template<typename KeyType, typename ValueType, size_t Size>
    struct ConstexprMap
    {

        std::array<std::pair<KeyType, ValueType>, Size> m_data;

        [[nodiscard]] constexpr ValueType at(const KeyType& key) const {
            const auto itr = std::find_if(begin(m_data), end(m_data),
                [&key](const auto& v) { return v.first == key; });
            if (itr != end(m_data)) {
                return itr->second;
            }
            else {
                throw std::range_error("Fake constexpr map out of range!");
            }
        }

        [[nodiscard]] constexpr bool contains(const KeyType& key) const {
            const auto itr = std::find_if(begin(m_data), end(m_data),
                [&key](const auto& v) { return v.first == key; });
            if (itr != end(m_data)) {
                return true;
            }
            else {
                return false;
            }
        }

    };
}