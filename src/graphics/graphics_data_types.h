#pragma once

constexpr int kBoardFirstLayerColorR = 128;
constexpr int kBoardFirstLayerColorG = 128;
constexpr int kBoardFirstLayerColorB = 128;

constexpr int kBoardSecondLayerColorR = 255;
constexpr int kBoardSecondLayerColorG = 198;
constexpr int kBoardSecondLayerColorB = 12;
constexpr int kBackSecondColorAlpha = 250;

struct GraphicMovementFrame {
    GraphicMovementFrame(float offset, sf::Time time, float targetEps) : elapsedTime(time), movementOffset(offset), targetEpsilon(targetEps) {}
    sf::Time elapsedTime;
    float distancePassedPercent{ 0.f };
    float movementOffset;
    float targetEpsilon;
    const float fullPath{ 1.f };
};



//using BoardSecondLayerColor = const sf::Color::(100, 9, 12, 250);