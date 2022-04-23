#include "projectile.h"

#include "game_tooltip.h"
#include <memory>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <cassert>

constexpr float kMoveOffsetPerFrame = 0.02f;
constexpr float kMoveTargetEps = 0.05f;
constexpr float kTooltipAnimationTime = 3;

graphics::Projectile::Projectile(const sf::Vector2f& sourcePoint, const sf::Vector2f& target, ISceneNode* parent, const  TextureHolder& textures) :
    m_textures(textures), m_sourcePoint(sourcePoint), m_target(target), m_moveFrame(kMoveOffsetPerFrame, sf::Time::Zero, kMoveTargetEps)
{
    m_sprite.setTexture(m_textures.get(textures::ID::Missile));
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin( bounds.width / 2.f,  bounds.height / 2.f);
    m_parent = parent;
    setPosition(sourcePoint);
}

void graphics::Projectile::setScale(const sf::Vector2f& factors) 
{
    m_sprite.setScale(factors);
}

void graphics::Projectile::updateCurrent(sf::Time dt)
{
    //m_moveFrame.elapsedTime += dt;
    if (m_isExploded) return;

    //sf::Time timePerFrame = sf::seconds(0.0f);

    m_sprite.setPosition(interpolateNextPoint());
    m_moveFrame.distancePassedPercent += m_moveFrame.movementOffset;
    //m_moveFrame.elapsedTime -= timePerFrame;

    if (m_moveFrame.fullPath - m_moveFrame.distancePassedPercent <= m_moveFrame.targetEpsilon)
    {
        m_isExploded = true;
        // TODO prob check in destructor that damage was applied to unit
        m_targetUnit->showDamage(m_targetDamage);
        //auto animatedTooltip = std::make_unique<AnimatedTooltip>(m_textures, kTooltipAnimationTime);
        //animatedTooltip->setText(std::string(m_targetDamage), m_targetUnit->getPosition());
        //attachChild(std::move(animatedTooltip));
    }
}

sf::Vector2f graphics::Projectile::interpolateNextPoint() 
{
    if (m_moveFrame.distancePassedPercent > 1.f)
        m_moveFrame.distancePassedPercent = 1.f;

    else if (m_moveFrame.distancePassedPercent < 0.f)
        m_moveFrame.distancePassedPercent = 0.f;

    return m_sourcePoint + (m_target - m_sourcePoint) * m_moveFrame.distancePassedPercent;
}

void graphics::Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!m_isExploded && !m_parent->getBoundingRect().intersects(m_sprite.getGlobalBounds()))
        target.draw(m_sprite);
}
