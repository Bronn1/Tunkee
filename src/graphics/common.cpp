#include "common.h"

sf::Vector2f graphics::getPointBetween(const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, float distancePercent)
{
    if (distancePercent > 1.f)
        distancePercent = 1.f;

    else if (distancePercent < 0.f)
        distancePercent = 0.f;

    return startPoint + (endPoint - startPoint) * distancePercent;
}

sf::Vector2f graphics::getBackDirectionOffset(const float objectRotation, const float offset)
{
    if (objectRotation >= 260.f && objectRotation < 280.f)
        return { 0.f, -offset };
    else if (objectRotation >= 280.f && objectRotation < 350.f)
        return { offset, -offset };
    else if ((objectRotation >= 350.f && objectRotation <= 360.f) ||
        (objectRotation >= 0.f && objectRotation < 10.f))
        return { offset, 0.f };
    else if (objectRotation >= 10.f && objectRotation < 80.f)
        return { offset, offset };
    else if (objectRotation >= 80.f && objectRotation < 100.f)
        return { 0.f, offset };
    else if (objectRotation >= 100.f && objectRotation < 170.f)
        return { -offset, offset };
    else if (objectRotation >= 170.f && objectRotation < 190.f)
        return { -offset, 0 };
    else
        return { -offset, -offset };
}
