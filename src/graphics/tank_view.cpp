//
// Created by ä on 06.11.2021.
//

#include "tank_view.h"

#include <numbers>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Clock.hpp>

constexpr float kDefaultUnitRotation = 270.f;
constexpr float kDefaultUnitScale = 0.17f;
constexpr int   kBasicVelocity = 40;
constexpr float kTurrentCenterPointDividerX = 1.25;
//using ProjectilePtr = std::unique_ptr<Projectile>;
constexpr float moveOffsetPerFrame = 0.02f;
constexpr float moveTargetEps = 0.f;

graphics::UnitView::UnitView(UnitIdentifier id, Type type, TextureHolder& textures) :
	m_type(type), m_bodySprite(textures.get(textures::ID::T34TankBody)),
	m_turretSprite(textures.get(textures::ID::T34TankTurret)), m_explosion(textures.get(textures::ID::Explosion)), m_explosion2(textures.get(textures::ID::Explosion)),
	m_textures(textures), m_moveFrame(moveOffsetPerFrame, sf::Time::Zero, moveTargetEps)
{
	m_id = id;
	m_explosion.setFrameSize(sf::Vector2i(256, 256));
	m_explosion.setNumFrames(16);
	m_explosion.setDuration(sf::seconds(1.f));
	m_explosion.centerOrigin();
	m_explosion.setScale({ 2.f,2.f });


	m_explosion2.setFrameSize(sf::Vector2i(256, 256));
	m_explosion2.setNumFrames(2);
	m_explosion2.setDuration(sf::seconds(0.8f));
	m_explosion2.centerOrigin();
	m_explosion2.setScale({ 1.8f,2.3f });

	sf::FloatRect bounds = m_bodySprite.getLocalBounds();
	sf::FloatRect turretBounds = m_turretSprite.getLocalBounds();
	// setting up center point
	m_turretSprite.setOrigin(turretBounds.width / kTurrentCenterPointDividerX, turretBounds.height / 2.f);
	m_bodySprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	auto [x, y] = m_bodySprite.getPosition();
	m_turretSprite.setPosition(x, y);
	m_tooltipDescription.setParentBounds(bounds);

	setRotation(kDefaultUnitRotation);
	setScale(kDefaultUnitScale, kDefaultUnitScale);

	//EntityView::setVelocity({ kBasicVelocity, kBasicVelocity });
}

//const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

void graphics::UnitView::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!m_isDestroyed)
	{
		target.draw(m_bodySprite, states);
		target.draw(m_turretSprite, states);
		target.draw(m_tooltipDescription, states);
		target.draw(m_tooltipDescription.getTextToRender(), states);
	}
	else
	{
		target.draw(m_bodySprite, states);
		//target.draw(m_explosion2, states);
		target.draw(m_explosion, states);
	}
}

void graphics::UnitView::updateCurrent(sf::Time dt)
{
	m_moveFrame.elapsedTime += dt;
	
	if (!m_movementPath.empty())
	{
		if ( m_moveFrame.fullPath - m_moveFrame.distancePassedPercent > 0 )
		{
			auto targetPoint = m_movementPath.top();
			setPosition(interpolateWithFactor(m_posBeforeMovement, targetPoint, m_moveFrame.distancePassedPercent));
			m_moveFrame.distancePassedPercent += m_moveFrame.movementOffset;
		}
		else
		{
			m_moveFrame.distancePassedPercent = 0.f;
			m_posBeforeMovement = getPosition();
			m_movementPath.pop();
			if (!m_movementPath.empty()) 
			{
				auto newTargetPoint = m_movementPath.top();
				rotateTo(m_posBeforeMovement, newTargetPoint);
			}
			else
			{
				markAsSelected();
				m_isPerformingAction = false;
				std::cout << getRotation() << "  GYI\n";
			}
		}
	}

	if (m_isDestroyed && !m_explosion.isFinished())
	{
		m_explosion.update(dt);
		m_explosion2.update(dt);	
	}
		
}

sf::FloatRect graphics::UnitView::getBoundingRect() const
{
	return getWorldTransform().transformRect(m_bodySprite.getGlobalBounds());
}

Angle graphics::UnitView::rotateGunTo(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint)
{
	float dx = curPoint.x - targetPoint.x;
	float dy = curPoint.y - targetPoint.y;

	float turretTextureOffset = 90;
	float rotation = ((atan2(dy, dx)) * 180.f / std::numbers::pi) + (360.f - getRotation());
	m_turretSprite.setRotation(rotation);
	std::cout << Angle{ m_turretSprite.getRotation() - (360.f - getRotation()) }.angle << " gyu angle\n";
	return  Angle{ m_turretSprite.getRotation() - (360.f - getRotation()) };
}

sf::Vector2f graphics::UnitView::getGunPeakPosition()
{
	return sf::Vector2f();
}

void graphics::UnitView::rotateTo(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint)
{
	float dx = curPoint.x - targetPoint.x;
	float dy = curPoint.y - targetPoint.y;

	float rotation = ((atan2(dy, dx)) * 180 / std::numbers::pi);
	setRotation(rotation);
	float turretTextureOffset = 0;
	m_turretSprite.setRotation(turretTextureOffset);
}

void graphics::UnitView::markAsSelected()
{
	if (m_isSelected) {
		m_bodySprite.setColor(sf::Color::White);
		m_turretSprite.setColor(sf::Color::White);
	}
	else {
		sf::Color selectionColor(224, 255, 255, 200);
		m_bodySprite.setColor(selectionColor);
		m_turretSprite.setColor(selectionColor);
	}
	m_isSelected = m_isSelected ? false : true;
}

void graphics::UnitView::showTooltip(const sf::Vector2f& mouse_pos) 
{
	if (getBoundingRect().contains(mouse_pos)) {
		m_tooltipDescription.show();
	}
	else {
		m_tooltipDescription.hide();
	}
}

graphics::SceneNodePtr graphics::UnitView::shot(const sf::Vector2f& destination)  
{
	const float projectileScale = 0.3f;
	float midCoordinatesX = m_bodySprite.getOrigin().x - (m_bodySprite.getLocalBounds().width / 2.f);//std::sin(m_turretSprite.getRotation()) + m_turretSprite.getLocalBounds().width;
	float midCoordinatesY = m_bodySprite.getOrigin().y - (m_bodySprite.getLocalBounds().height / 2.f);//std::cos(m_turretSprite.getRotation()) + m_turretSprite.getLocalBounds().width;
	auto projectile = std::make_unique<Projectile>(getTransform().transformPoint({ midCoordinatesX, midCoordinatesY}), destination, this, m_textures.get(textures::ID::Missile));
	projectile->setScale({ projectileScale, projectileScale });
	projectile->setTarget(destination);
	projectile->setRotation(Angle{ m_turretSprite.getRotation() - (360.f - getRotation()) }.angle);

	return projectile;
}

void graphics::UnitView::setMovementState(std::stack<sf::Vector2f> path)
{
	if (path.empty()) return;
	if (m_isSelected) markAsSelected();

	m_movementPath = std::move(path);
	auto targetPoint = m_movementPath.top();
	m_posBeforeMovement = getPosition();
	rotateTo(m_posBeforeMovement, targetPoint);
	m_isPerformingAction = true;
}
