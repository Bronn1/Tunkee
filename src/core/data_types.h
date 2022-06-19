#pragma once

#include "game_tile.h"

#include <compare>
#include <vector>
#include <cmath>


/**
* @brief Represents vertex of hexagon
* 
*     0   
*   /   \
*  5     1
*  |     |
*  4     2
*   \   /
*     3 
*/
struct HexVertexNumber {
    explicit HexVertexNumber(int vertex) : vertexNum(vertex) {
        if (vertex > 5) vertexNum = vertex % 6;
        if (vertex < 0) vertexNum = 6 - (vertex % 6);
    }

    int vertexNum;
    bool operator==(const HexVertexNumber&) const = default;
    auto operator<=>(const HexVertexNumber&) const = default;
    //inline operator bool() const { return identifier != 0; }
};

enum class HexVertex : unsigned {
    Vertex0 = 0,
    Vertex1 = 1,
    Vertex2 = 2,
    Vertex3 = 3,
    Vertex4 = 4,
    Vertex5 = 5,
};

/**
* @brief Unit Identifier for every game entity 
*/
struct UnitIdentifier {
    unsigned int identifier;
    bool operator==(const UnitIdentifier&) const = default;
    auto operator<=>(const UnitIdentifier&) const = default;
    inline operator bool() const { return identifier != 0; }
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
    int m_frontal;
    int m_side;
    //bool operator==(const RateOfFire&) const = default;
    auto operator<=>(const Armor&) const = default;
    Armor(const unsigned frontal, const unsigned side) : m_frontal(frontal % kMaxArmorFronatal), m_side(side % kMaxSide) {}
};

struct Attack {
     int attack;
    auto operator<=>(const Attack&) const = default;
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

enum  class PointOfView {
    Player,
    Enemy
};

enum class ActionStatus
{
    empty = 0 ,
    full = 1
};

namespace setup
{
/**
 * @brief money to buy units at setup(deployment) phase of quick game and multiplayer
*/
struct Money
{
    short amount;

    auto operator<=>(const Money&) const = default;
    auto& operator+=(const Money& other) { amount += other.amount; return *this; };
    auto& operator-=(const Money& other) { amount -= other.amount; return *this; };
    inline operator bool() const { return amount != 0; }
};
}

template< typename T>
struct Comparator {
    bool operator() (const T& a, const  T& b) const {
        auto [l] = a;
        auto [r] = b;
        return l < r;
    }

};

struct ActionState {
    TileDistance m_fullMovePoints{ 0 };
    Shots m_rateOfFire{ 0 };
    TileDistance m_remainingMovePoints{ 0 };
    Shots m_remainingShots{ 0 };
};

/**
 * @brief Helps to understand  unit movement pattern for other components like
 * damage calculator( example: if unit moving with full speed its a less chance to hit him),
 * for logger and then show it properly to player
 * We cant use just action state cuz unit can be damaged so can not move because of it but didnt do any movement
*/
enum class UnitMoveStatus{ 
    Idleing,
    MovingHalfSpeed,
    MovingFullSpeed,
};

enum class ActiveStatus {
    Inactive = 0,
    OneAction,
    TwoActions
};

/**
 * @brief  Actions unit made during his 'last turn'(check logger descriprion for it) 
 * Idle by default
*/
struct LastActions {
    UnitMoveStatus moveAction{ UnitMoveStatus::Idleing };
    ActiveStatus activeStatus{ ActiveStatus::Inactive };
};

namespace std {
template<> struct hash<UnitIdentifier> {
    size_t operator()(const UnitIdentifier& id) const {
        return std::hash<unsigned>()(id.identifier);
    }
};

template<> struct hash<PlayerIdentifier> {
    size_t operator()(const PlayerIdentifier& id) const{
        return std::hash<unsigned>()(id.identifier);
    }
};
}

Angle VertexToAngle(HexVertexNumber vertex); 
HexVertexNumber AngleToClosestVertex(Angle angle);