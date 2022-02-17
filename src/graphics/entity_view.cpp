#include "entity_view.h"


void graphics::EntityView::setVelocity(const sf::Vector2f& velocity)
{
    m_velocity = velocity;
}

void graphics::EntityView::setVelocity(float vx, float vy)
{
    m_velocity.x = vx;
    m_velocity.y = vy;
}

sf::Vector2f graphics::EntityView::getVelocity() const
{
    return m_velocity;
}

void graphics::EntityView::updateCurrent(sf::Time dt)
{
    move(m_velocity * dt.asSeconds());
}
