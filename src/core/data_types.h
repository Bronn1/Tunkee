#pragma once

#include "game_tile.h"

#include <compare>
#include <vector>

/**
* Object Identifier for every game entity 
*/
struct ObjectIdentifier {
	unsigned int identifier;
	bool operator==(const ObjectIdentifier&) const = default;
};

struct HealthPoints {
	unsigned int points;
};

struct PlayerIdentifier {
	unsigned int identifier;
	bool operator==(const PlayerIdentifier&) const = default;
};

struct TileDistance {
	unsigned int distance;
	bool operator==(const TileDistance&) const = default;
	TileDistance operator+=(const TileDistance& d) { distance += d.distance; return *this; }
	auto operator<=>(const TileDistance&) const = default;
};


struct Shots {
	unsigned int shots;
	//bool operator==(const RateOfFire&) const = default;
	auto operator<=>(const Shots&) const = default;
};

struct MoveAreaAndFirstLayerSize {
	MoveAreaAndFirstLayerSize() = default;
	MoveAreaAndFirstLayerSize(std::vector<core::GameTile> area, int half): moveArea(area), firstLayerSize(half) {}
	MoveAreaAndFirstLayerSize(MoveAreaAndFirstLayerSize&&) = default;
	std::vector<core::GameTile> moveArea{};
	int firstLayerSize{ 0 };
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