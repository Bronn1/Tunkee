//
// Created by ä on 06.11.2021.
//

#include "tank_view.h"
#include "common.h"

#include <numbers>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Clock.hpp>

constexpr float kDefaultUnitRotation = 270.f;
constexpr float kDefaultUnitScale = 0.17f;
constexpr float kTurrentCenterPointDividerX = 1.25;
constexpr float kMoveOffsetPerFrame = 0.01f;
constexpr float kMoveBackAnimationOffset = 0.1f;
constexpr float kMoveTargetEps = 0.f;

// TODO not sure where better put these constants 
using namespace std::literals::string_view_literals;
static constexpr std::array<std::pair<tankDamageSystem::DamageTo, textures::ID>, 7> kDamageIconsArr{ {{tankDamageSystem::kLoader, textures::ID::LoaderKilledIcon},
 { tankDamageSystem::kExploded, textures::ID::ExplodedIcon},{ tankDamageSystem::kBurning, textures::ID::BurningIcon} ,{ tankDamageSystem::kGunDestroyed, textures::ID::GunDestroyedIcon},
 { tankDamageSystem::kCrewShellShocked, textures::ID::CrewShellshockedIcon}, { tankDamageSystem::kEngine, textures::ID::EngineDestroyedIcon}, 
 { core::kHiddenDamage, textures::ID::HiddenDamageIcon}} };
static constexpr auto kDamageIcons = graphics::ConstexprMap<tankDamageSystem::DamageTo, textures::ID, kDamageIconsArr.size()>{ {kDamageIconsArr} };

graphics::TankView::TankView(UnitIdentifier id, Type type, TextureHolder& textures) :
    m_type(type), m_bodySprite(textures.get(textures::ID::T34TankBody)), IUnitView(id),
    m_turretSprite(textures.get(textures::ID::T34TankTurret)), m_explosion(textures.get(textures::ID::Explosion)), m_buriningAnimation(textures.get(textures::ID::Explosion)),
    m_textures(textures), m_moveFrame(kMoveOffsetPerFrame, sf::Time::Zero, kMoveTargetEps), m_tooltipDescription(textures)
{
    // TODO after we'll have full animation support get rid of magic numbers here
    m_explosion.setFrameSize(sf::Vector2i(256, 256));
    m_explosion.setNumFrames(16);
    m_explosion.setDuration(sf::seconds(1.5f));
    m_explosion.centerOrigin();
    m_explosion.setScale({ 2.4f,2.4f });

    m_buriningAnimation.setFrameSize(sf::Vector2i(256, 256));
    m_buriningAnimation.setNumFrames(4);
    m_buriningAnimation.setDuration(sf::seconds(0.5f));
    m_buriningAnimation.centerOrigin();
    m_buriningAnimation.setScale({ 0.5f, 0.5f });
    m_buriningAnimation.setRepeating(true);


    sf::FloatRect bounds = m_bodySprite.getLocalBounds();
    sf::FloatRect turretBounds = m_turretSprite.getLocalBounds();
    // setting up center point
    m_turretSprite.setOrigin(turretBounds.width / kTurrentCenterPointDividerX, turretBounds.height / 2.f);
    m_bodySprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    auto [x, y] = m_bodySprite.getPosition();
    m_turretSprite.setPosition(x, y);
    m_buriningAnimation.move({bounds.width / 3, bounds.height / 4, });

    setRotation(kDefaultUnitRotation);
    setScale(kDefaultUnitScale, kDefaultUnitScale);
}

void graphics::TankView::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_isDestroyed)
    {
        target.draw(m_bodySprite, states);
        target.draw(m_turretSprite, states);
    }
    else
        target.draw(m_bodySprite, states);

    target.draw(m_tooltipDescription);
    target.draw(m_explosion, states);
    target.draw(m_buriningAnimation, states);
}

void graphics::TankView::updateCurrent(sf::Time dt)
{
    checkMoveState();
    updateMovement(dt);
    m_buriningAnimation.update(dt);
    m_explosion.update(dt);
}

float graphics::TankView::getRotationOffset(const float curRotation, const  float targetRotation)
{
    float changeDirrection = -1.f;
    float rotationOffset{ 0.9f };
    
    if (abs(curRotation - targetRotation) > 180.f)
        changeDirrection = 1.f;

    float resultOffset = rotationOffset;
    if (curRotation - targetRotation <= 0.f)
        resultOffset = -rotationOffset;
    else
        resultOffset = rotationOffset;

    resultOffset *= changeDirrection;
    return resultOffset;
}

void graphics::TankView::updateMovement(sf::Time dt)
{
    m_moveFrame.elapsedTime += dt;
    switch (m_currentState)
    {
    case MoveState::Rotating:

        if (m_moveBackAnimationPeriod > 0)
        {
            move(getBackDirectionOffset(getRotation(), kMoveBackAnimationOffset));
            --m_moveBackAnimationPeriod;
            break;
        }

        rotate(m_rotationAnimationOffset);
        if (abs(m_targetRotation.angle - getRotation()) <= abs(m_rotationAnimationOffset))
        {
            m_currentState = MoveState::Idle;
            checkMoveState();
            setRotation(m_targetRotation.angle);
        }
        break;
    case MoveState::TurretRotating:
        m_turretSprite.rotate(m_rotateTurretAnimationOffset);
        if (abs(m_turretTargetRotation.angle - m_turretSprite.getRotation()) <= abs(m_rotateTurretAnimationOffset))
        {
            m_currentState = MoveState::Idle;
            m_turretSprite.setRotation(m_turretTargetRotation.angle);
            if (m_delayedTarget)
                shot(m_delayedTarget, m_delayedDamage);
        }
        break;

    case  MoveState::Moving:
        if (m_moveFrame.fullPath - m_moveFrame.distancePassedPercent > 0)
        {
            auto targetPoint = m_movementPath.top();
            setPosition(getPointBetween(m_posBeforeMovement, targetPoint, m_moveFrame.distancePassedPercent));
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

                setPosition(getPointBetween(m_posBeforeMovement, newTargetPoint, m_moveFrame.distancePassedPercent));
                m_moveFrame.distancePassedPercent += m_moveFrame.movementOffset;
                setPosition(getPointBetween(m_posBeforeMovement, newTargetPoint, m_moveFrame.distancePassedPercent));
                m_moveFrame.distancePassedPercent += m_moveFrame.movementOffset;
                setPosition(getPointBetween(m_posBeforeMovement, newTargetPoint, m_moveFrame.distancePassedPercent));
                m_moveFrame.distancePassedPercent += m_moveFrame.movementOffset;
                setPosition(getPointBetween(m_posBeforeMovement, newTargetPoint, m_moveFrame.distancePassedPercent));
                m_moveFrame.distancePassedPercent += m_moveFrame.movementOffset;
                setUpRotationAnimation(m_posBeforeMovement, newTargetPoint);
            }
            else
            {
                markAsSelected();
                setPerformingAction(false);
                setUpRotationAnimation(getPosition(), m_currentRotationPoint);
                std::cout << getRotation() << "  GYI\n";
            }
        }
        break;
    }
}

sf::FloatRect graphics::TankView::getBoundingRect() const
{
    return getWorldTransform().transformRect(m_bodySprite.getGlobalBounds());
}

Angle graphics::TankView::calculateGunRotation(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint) const
{
    float dx = curPoint.x - targetPoint.x;
    float dy = curPoint.y - targetPoint.y;
    float rotation = ((atan2(dy, dx)) * 180.f / std::numbers::pi);
    
    return  Angle{ rotation };
}

void graphics::TankView::setUpGunRotationAnimation(const Angle& gunRotation)
{
    // here we also add texture offset so turret peak will point to correct direction: 360.f - getRotation()
    float turretTextureOffset = 360.f - getRotation();
    m_turretTargetRotation = Angle{ gunRotation.angle + turretTextureOffset };
    if (abs(m_turretTargetRotation.angle - m_turretSprite.getRotation()) < 0.5f) {
        m_turretSprite.setRotation(m_turretTargetRotation.angle);
        return;
    }
    m_currentState = MoveState::TurretRotating;
    m_rotateTurretAnimationOffset = getRotationOffset(m_turretSprite.getRotation(), m_turretTargetRotation.angle);
    //m_turretSprite.setRotation(gunRotation.angle + turretTextureOffset);
    std::cout << Angle{ m_turretSprite.getRotation() - turretTextureOffset }.angle << " gyu angle\n";
}

void graphics::TankView::setUpRotationAnimation(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint)
{
    float dx = curPoint.x - targetPoint.x;
    float dy = curPoint.y - targetPoint.y;
    float rotation = ((atan2(dy, dx)) * 180 / std::numbers::pi);
    
    m_targetRotation = Angle{ rotation };
    if (abs(m_targetRotation.angle - getRotation()) <  5.f)
    {
        m_currentState = MoveState::Idle;
        checkMoveState();
        setRotation(rotation);
        return;
    }

    float curRotation = getRotation();
    m_currentState = MoveState::Rotating;
    m_rotationAnimationOffset = getRotationOffset(getRotation(), m_targetRotation.angle);
    m_moveBackAnimationPeriod = kMoveBackAnimationPeriod;
}

void graphics::TankView::markAsSelected()
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

void graphics::TankView::showTooltip(const sf::Vector2f& mouse_pos) 
{
    if (getBoundingRect().contains(mouse_pos)) 
        m_tooltipDescription.show();
    else
        m_tooltipDescription.hide();
}

void graphics::TankView::showDamage(std::string_view damageType)
{
    // get rid of if else if blocks, come up with smth more brilliant(needed first full understanding how many animation we'll have)
    if (damageType == tankDamageSystem::kExploded)
    {
        m_isDestroyed = true;
        m_buriningAnimation.setScale({ 0.9f, 0.9f });
        m_explosion.startAnimation();
    }
    if (damageType == tankDamageSystem::kBurning)
        m_buriningAnimation.startAnimation();
}

void graphics::TankView::shot(IUnitView* target, std::string_view damageType)
{
    if (m_currentState != MoveState::Idle) {
        m_delayedDamage = damageType;
        m_delayedTarget = target;
        return;
    }

    const float projectileScale = 0.3f;
    float midCoordinatesX = m_bodySprite.getOrigin().x - (m_bodySprite.getLocalBounds().width / 2.f);//std::sin(m_turretSprite.getRotation()) + m_turretSprite.getLocalBounds().width;
    float midCoordinatesY = m_bodySprite.getOrigin().y - (m_bodySprite.getLocalBounds().height / 2.f);//std::cos(m_turretSprite.getRotation()) + m_turretSprite.getLocalBounds().width;
    auto projectile = std::make_unique<Projectile>(getTransform().transformPoint({ midCoordinatesX, midCoordinatesY }), target->getPosition(), this, m_textures);
    projectile->setScale({ projectileScale, projectileScale });
    projectile->setTarget(target->getPosition());
    projectile->setDamageType(damageType);
    projectile->setRotation(Angle{ m_turretSprite.getRotation() - (360.f - getRotation()) }.angle);
    projectile->setTargetUnit(target);
    attachChild(std::move(projectile));
    m_delayedDamage = "";
    m_delayedTarget = nullptr;
}

void graphics::TankView::recoveryDamage(std::string_view damageType)
{
    if (damageType == tankDamageSystem::kBurning) {
        m_buriningAnimation.stopAnimation();
    }
}

void graphics::TankView::setMovementPath(std::stack<sf::Vector2f> path)
{
    if (path.empty()) return;
    if (m_isSelected) markAsSelected();

    m_movementPath = std::move(path);
    auto targetPoint = m_movementPath.top();
    m_posBeforeMovement = getPosition();
    setUpRotationAnimation(m_posBeforeMovement, targetPoint);
    setPerformingAction(true);
}

void graphics::TankView::checkMoveState()
{
    if (m_currentState == MoveState::Idle && !m_movementPath.empty())
        m_currentState = MoveState::Moving;
}

void graphics::TankView::setTooltipMsg(bool isAlive, const  std::vector<std::string>& unitInfo, const std::vector<tankDamageSystem::DamageTo>& damageToVec)
{
    std::vector<textures::ID> iconIDs{};
    for (auto& damage : damageToVec)
        if (kDamageIcons.contains(damage))
            iconIDs.push_back(kDamageIcons.at(damage));

    auto coord = getPosition();
    m_tooltipDescription.setContent(isAlive, unitInfo, iconIDs, coord);
}
