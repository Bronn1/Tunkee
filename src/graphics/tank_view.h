#pragma once

#include "interface_unit_view.h"
#include "game_tooltip.h"
#include "explosion_effects.h"
#include "resource_holder.h"
#include "projectile.h"


#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>


namespace graphics
{
    constexpr int kMoveBackAnimationPeriod = 50;
    
     // TODO add animation when unit move slightly with unrotatable turret to shoot
    using SceneNodePtr = std::unique_ptr<ISceneNode>;

    class TankView : public IUnitView
    {
    public:
        enum class Type
        {
            Basic
        };
        enum class MoveState {
            Idle,
            Moving,
            Rotating,
            TurretRotating
        };
    public:
        TankView(UnitIdentifier id, Type type, const  TextureHolder& textures);
        sf::FloatRect getBoundingRect() const override;
        Angle  calculateGunRotation(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint) const override;
        void shot(IUnitView* target, std::string_view damageType) override;
        void  markAsSelected() override;
        void setTooltipMsg(bool isAlive, const  std::vector<std::string>& unitInfo, const std::vector<tankDamageSystem::DamageTo>& damageToVec) override;
        void  showTooltip(const sf::Vector2f& mouse_pos) override;
        void showDamage(std::string_view damageType) override;
        void setUpGunRotationAnimation(const Angle& gunRotation) override;
        void setUpRotationAnimation(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint) override;
        void recoveryDamage( std::string_view damageType) override;
        void setMovementPath(std::stack < sf::Vector2f> path) override;
        void setCurrentRotationPoint(const sf::Vector2f& point) override  { m_currentRotationPoint = point; } ;
        void setPositionBeforeMovement(const sf::Vector2f& position) { m_posBeforeMovement = position; }
        void checkMoveState();
        virtual ~TankView() = default;
    private:
        void updateMovement(sf::Time dt);
        void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        void updateCurrent(sf::Time dt) override;
        float getRotationOffset(const float curRotation, const float targetRotation);
    private:
        Type m_type;
        sf::Sprite	m_bodySprite;
        sf::Sprite m_turretSprite;
        // later on after full animation support add maybe in array and remove after animation is done
        Animation  m_explosion;
        Animation  m_buriningAnimation;
        bool m_isDestroyed{ false };
        bool m_isSelected{ false };
        UnitInfoTooltip m_tooltipDescription;
        const TextureHolder& m_textures;
        GraphicMovementFrame m_moveFrame;
        MoveState m_currentState{ MoveState::Idle };
        Angle m_targetRotation{0.f};
        float  m_rotationAnimationOffset{ 0.9f };

        Angle m_turretTargetRotation{ 0.f };
        float  m_rotateTurretAnimationOffset{ 0.9f };
        int m_moveBackAnimationPeriod{ kMoveBackAnimationPeriod };
        std::stack<sf::Vector2f> m_movementPath{};
        sf::Vector2f m_posBeforeMovement{ 0.f, 0.f };
        sf::Vector2f m_currentRotationPoint{ 0.f, 0.f };
        IUnitView* m_delayedTarget{ nullptr }; // nullunitview
        std::string_view m_delayedDamage{ "" };
    };

    class NullUnitView : public TankView
    {
    public:
        NullUnitView(TextureHolder& textures) : m_emptyTexture(), TankView(UnitIdentifier{ 0 }, Type::Basic, textures) {  }
    private:
        void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override {}
        Angle  calculateGunRotation(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint) const  override { return Angle{ 0 }; }
        //void  rotateTo(const sf::Vector2f& curPoint, const sf::Vector2f& targetPoint)  {}
        void shot(IUnitView* target, std::string_view damageType) override { return ; }

        sf::Texture m_emptyTexture;
    };
}
