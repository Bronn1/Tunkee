#pragma once

#include "game_tile.h"

#include <compare>
#include <vector>

/**
* Object Identifier for every game entity 
*/
struct UnitIdentifier {
	unsigned int identifier;
	bool operator==(const UnitIdentifier&) const = default;
	auto operator<=>(const UnitIdentifier&) const = default;
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
	auto operator+=(const TileDistance& other) { distance += other.distance; return *this; }
	auto operator-=(const TileDistance& other) { distance -= other.distance; return *this; }
	auto operator+ (const TileDistance & other) const { return TileDistance{ distance + other.distance }; }
	auto operator- (const TileDistance& other) const { return TileDistance{ distance - other.distance }; }
	auto operator<=>(const TileDistance&) const = default;
};

struct Shots {
	unsigned int shots;
	//bool operator==(const RateOfFire&) const = default;
	auto operator<=>(const Shots&) const = default;
	auto operator+ (const Shots& other) const { return TileDistance{ shots + other.shots }; }
	auto operator- (const Shots& other) const { return TileDistance{ shots - other.shots }; }
};

enum class ActionStateStatus
{
	empty = 1,
	half,
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