#include "projectile.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <cassert>

constexpr float moveOffsetPerFrame = 0.05f;
constexpr float moveTargetEps = 0.05f;

graphics::Projectile::Projectile(const sf::Vector2f& sourcePoint, const sf::Vector2f& target, SceneNode* parent, const sf::Texture& texture) :
	m_sprite(texture), m_sourcePoint(sourcePoint), m_target(target), m_moveFrame(moveOffsetPerFrame, sf::Time::Zero, moveTargetEps)
{
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

	//sf::Time timePerFrame = sf::seconds(0.0f);

	m_sprite.setPosition(interpolateNextPoint());
	m_moveFrame.distancePassedPercent += m_moveFrame.movementOffset;
	//m_moveFrame.elapsedTime -= timePerFrame;

	if (m_moveFrame.fullPath - m_moveFrame.distancePassedPercent <= m_moveFrame.targetEpsilon)
	{
		m_isExploded = true;
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

void graphics::Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(!m_isExploded && !m_parent->getBoundingRect().intersects(m_sprite.getGlobalBounds()))
		target.draw(m_sprite, states);
}
