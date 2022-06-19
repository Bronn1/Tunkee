#pragma once

#include <cassert>
#include <iostream>

namespace core {
    enum class GameTileType
    {
        Grass = 1,
        Water = 2,
        Test3 = 3,
        Test5 = 5,
        Shrub = 6,
        Woods = 7,
        UnshootableWall
    };

    /**
    * Represents game tile data structure with Axial coordinate system
    */
    struct GameTile {
        int m_q{0};
        int m_r{ 0 };
        int m_s{ 0 };
        GameTileType m_type{ GameTileType::Grass };
        bool isAccessible{ true };

        
        bool isShootableThrough() const  {
            return (m_type != GameTileType::UnshootableWall) ? true : false;
        }

        GameTile() = default;
        GameTile(int q, int r) : m_q(q), m_r(r), m_s(-q - r), m_type(GameTileType::Grass) { assert(m_q + m_r + m_s == 0); }
        inline int travelCost() const { return static_cast<int>(m_type); }

        // operators to work with tile coordinates
        auto operator == (const GameTile& b) const {
            return (this->m_q == b.m_q && this->m_r == b.m_r && this->m_s == b.m_s);
        }
        auto operator != (const GameTile& b) const {
            return !(this->m_q == b.m_q && this->m_r == b.m_r && this->m_s == b.m_s);
        }
        auto operator + (const GameTile& b) const   {
            return GameTile(this->m_q + b.m_q, this->m_r == b.m_r);
        }
        auto operator - (const GameTile& b) const {
            return GameTile(this->m_q - b.m_q, this->m_r - b.m_r);
        }
        auto operator * (const int k) const {
            return GameTile(this->m_q * k , this->m_r * k);
        }

        friend std::ostream& operator<<(std::ostream& os, const GameTile& gt);
    };

    std::ostream& operator<<(std::ostream& os, const GameTile& gt);

    GameTile NullTile();
}

namespace std {
    // hash function to have an ability to add GameTile to containers like unordered map
    template<> struct hash<core::GameTile> {
        size_t operator()(const core::GameTile& h) const{
            hash<int> hash;
            size_t hq = hash(h.m_q);
            size_t hr = hash(h.m_r);
            return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
        }
    };
}