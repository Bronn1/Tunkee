#pragma once

#include "tank_view.h"
#include "board_view.h"
#include "../src/core/data_types.h"
#include "../controllers/game_controller.h"
#include "unit_setup_view.h"
#include "projectile.h"

#include <map>
#include <memory>

class GameBuilder;

namespace graphics {
    using SceneNodePtr = std::unique_ptr<graphics::SceneNode>;
    using ProjectilePtr = std::unique_ptr<graphics::Projectile>;
    using UnitViewPtr = std::unique_ptr < graphics::UnitView>;

    class GameWorldView : public events::Observer<core::GameEngine>
    {
    public:
        // TODO make constructor private and let create only via builder
        GameWorldView(sf::RenderWindow& target, BoardView board, controllers::GameController controller);
        bool addNewUnitView(UnitViewPtr unit, core::GameTile position);
        void addNullUnit(UnitViewPtr nullUnit);
        void draw();
        void update(sf::Event& event);
    private:
        void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos);
        void moveView(const sf::Event& event, const sf::Vector2f& mousePos);
        bool checkIfClickedOnUnit(const sf::Vector2f& mousePos);
        void checkMousePosition(const sf::Vector2f& mousePos);
        void onBoardClicked(const sf::Vector2f& mousePos);
        void clearMoveArea();
        void endSetupStage();

        // observer events
        void newUnitSelected(const UnitSelectedInfo& unitInfo) override;
        void informationMsgRecieved(const core::UnitStateMsg& unitStateMs) override;
        void moveAreaRecieved(const MoveAreaInfo& moveArea) override;
        void moveUnitRecieved(const MoveUnitInfo& moveUnit) override;
        void ChangeUnitStateRecieved(const UnitStateInfo& shotUnit) override;
        void rotateGunRecieved(const RotateGunInfo& rotateUnitGun) override;
        void setUnitSetupController(controllers::UnitSetupContoller controller) { m_unitsSetupView.setUnitSetupController(std::move(controller)); }
        friend class ::GameBuilder;
    private:
        sf::RenderWindow& m_renderTarget;
        BoardView m_board;
        std::map<UnitIdentifier, UnitViewPtr, Comparator<UnitIdentifier>>  m_units{};
        std::vector< SceneNodePtr> m_views;
        PlayerIdentifier m_playerId{ 1 };
        UnitView*  m_selectedUnit;
        bool m_isPerformingAction{ false };
        controllers::GameController m_gameController;

        UnitSetupView m_unitsSetupView;
        bool m_isUnitSetupViewHided{ false };

        // TODO prob hide all view logic inside separate class
        sf::Vector2f m_mousePosBeforeMoving;
        sf::View     m_view;
        bool         m_isViewMoving{ false };
    };
}