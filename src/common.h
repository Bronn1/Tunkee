#pragma once


namespace common 
{
constexpr int kExitGameSession = 1;
/**
 * @brief Constexpr map based on std::array, gives significant performance increase compared  to usual map
 * 
 * GCC can convert it to look up table
 * @tparam KeyType
 * @tparam ValueType
*/
template<typename KeyType, typename ValueType, size_t Size>
struct ConstexprMap
{

    std::array<std::pair<KeyType, ValueType>, Size> m_data;

    [[nodiscard]] constexpr ValueType at(const KeyType& key) const
    {
        const auto itr = std::find_if(begin(m_data), end(m_data),
                                      [&key](const auto& v) { return v.first == key; });
        if (itr != end(m_data)) {
            return itr->second;
        } else {
            throw std::range_error("Fake constexpr map out of range!");
        }
    }

    [[nodiscard]] constexpr bool contains(const KeyType& key) const
    {
        const auto itr = std::find_if(begin(m_data), end(m_data),
                                      [&key](const auto& v) { return v.first == key; });
        if (itr != end(m_data)) {
            return true;
        } else {
            return false;
        }
    }

};
} // namespace common 