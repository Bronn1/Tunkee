#pragma once

#include <cassert>
#include <iostream>

namespace core {
	enum class GameTileType
	{
		grass = 1,
		water = 2,
		test3 = 3,
		test5 = 5
	};

	/**
	* Represents game tile data structure with Axial coordinate system
	*/
	struct GameTile {
		int m_q;
		int m_r;
		int m_s;
		GameTileType m_type;  
		bool isAccessible{ true };

		GameTile() : m_q(0), m_r(0), m_s(0), m_type(GameTileType::grass), isAccessible{ true } {}

		GameTile(int q, int r) : m_q(q), m_r(r), m_s(-q - r), m_type(GameTileType::grass) { assert(m_q + m_r + m_s == 0); }
		inline int staticCastTypeToInt() const { return static_cast<int>(m_type); }

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

		GameTile& operator=(const GameTile& other)
		{
			if (this == &other)
				return *this;
	
			this->m_q = other.m_q;
			this->m_r = other.m_r;
			this->m_s = other.m_s;
			this->m_type = other.m_type;
			this->isAccessible = other.isAccessible;
			return *this;
		}

		/*GameTile(const GameTile& other)
		{
			this->m_q = other.m_q;
			this->m_r = other.m_r;
			this->m_s = other.m_s;
			this->m_type = other.m_type;
			this->isAccessible = other.isAccessible;
		}*/
	};

	std::ostream& operator<<(std::ostream& os, const GameTile& gt);
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