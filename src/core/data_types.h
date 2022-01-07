#pragma once

#include "game_tile.h"

#include <compare>
#include <vector>
#include <cmath>

/**
* @brief Unit Identifier for every game entity 
*/
struct UnitIdentifier {
	unsigned int identifier;
	bool operator==(const UnitIdentifier&) const = default;
	auto operator<=>(const UnitIdentifier&) const = default;
	inline operator bool() const { return identifier != 0; }
};

struct HealthPoints {
	unsigned int points;
};

struct PlayerIdentifier {
	unsigned int identifier;
	//PlayerIdentifier& operator =(const PlayerIdentifier&) = default;
	bool operator==(const PlayerIdentifier&) const = default;
};

struct TileDistance {
	unsigned int distance;
	bool operator==(const TileDistance&) const = default;
	//auto operator-(const TileDistance& other) const { return TileDistance{ this->distance - other.distance }; }
	auto& operator+=(const TileDistance& other) { distance += other.distance; return *this; }
	auto& operator-=(const TileDistance& other) { distance -= other.distance; return *this; }
	auto operator+ (const TileDistance & other) const { return TileDistance{ distance + other.distance }; }
	auto operator- (const TileDistance& other) const { return TileDistance{ distance - other.distance }; }
	auto operator<=>(const TileDistance&) const = default;
};

struct Shots {
	unsigned int shots;
	//bool operator==(const RateOfFire&) const = default;
	auto operator<=>(const Shots&) const = default;
	auto operator+ (const Shots& other) const { return Shots{ shots + other.shots }; }
	auto operator- (const Shots& other) const { return Shots{ shots - other.shots }; }
	auto& operator-=(const Shots& other) { shots -= other.shots; return *this; }
};

constexpr unsigned int kMaxArmorFronatal = 6;
constexpr unsigned int kMaxSide = 6;

struct Armor {
	unsigned int m_frontal;
	unsigned int m_side;
	//bool operator==(const RateOfFire&) const = default;
	auto operator<=>(const Armor&) const = default;
	Armor(const unsigned frontal, const unsigned side) : m_frontal(frontal % kMaxArmorFronatal), m_side(side % kMaxSide) {}
};

struct Angle {
	float angle;
	const float kMaxAngle{ 360.f };

	 explicit Angle(float ang) {
		angle = fmod(ang, kMaxAngle);
		if (angle < 0) angle += kMaxAngle; 
	}

	auto operator<=>(const Angle&)  const = default;
	auto operator +(const Angle& other) const { return Angle{ angle + other.angle }; }
	auto operator -(const Angle& other) const { return Angle{ angle - other.angle }; }

	auto operator=(Angle other)  noexcept {
		swap(other);
		return *this;
	}

	void swap(Angle& second) noexcept {
		std::swap(this->angle, second.angle);
	}

	Angle(const Angle& other){
		angle = other.angle;
	}
};

enum class ActionStateStatus
{
	empty = 1,
	full
};

template< typename T>
struct Comparator {
	bool operator() (const T& a, const  T& b) const {
		auto [l] = a;
		auto [r] = b;
		return l < r;
	}

};