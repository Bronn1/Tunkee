#pragma once

#include "tank_view.h"
#include "board_view.h"
#include "button.h"
#include "src/core/data_types.h"
#include "src/controllers/game_controller.h"
#include "unit_setup_view.h"
#include "projectile.h"

#include <map>
#include <memory>

class GameBuilder;

namespace graphics {
    using SceneNodePtr = std::unique_ptr<graphics::ISceneNode>;
    using ProjectilePtr = std::unique_ptr<graphics::Projectile>;
    using UnitViewPtr = std::unique_ptr < graphics::IUnitView>;


    class GameWorldView : public events::Observer<core::GameEngine>
    {
    public:
        using UIElementPtr = std::unique_ptr < UIElement>;
        // TODO make constructor private and let create only via builder
        GameWorldView(sf::RenderWindow& target, BoardView board, controllers::GameController controller);
        
        bool addNewUnitView(UnitViewPtr unit, core::GameTile position);
        void addNullUnit(UnitViewPtr nullUnit);
        void draw();
        int handleEvent(sf::Event& event);
        void update(const sf::Time updateTime);
    private:
        void initUI();
        void switchEvent(const sf::Event& event, const sf::Vector2f& mousePos);
        void moveView(const sf::Event& event, const sf::Vector2f& mousePos);
        void resizeView(const float resizeFactor);
        bool checkIfClickedOnUnit(const sf::Vector2f& mousePos);
        void checkMousePosition(const sf::Vector2f& mousePos);
        void onBoardClicked(const sf::Vector2f& mousePos);
        void clearMoveArea();
        void endSetupStage();
        void setSetupArea(const std::vector<core::GameTile>& area) { m_board.selectSetupArea(area); }

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
        /** Contains some graphic parts like animations, some objects */
        std::vector< SceneNodePtr> m_graphicElements;
        /** Contains user interface like buttons, menu and every interactive panel*/
        std::vector< UIElementPtr > m_UIelements;
        PlayerIdentifier m_playerId{ 1 };
        /** initialized in game builder with null unit, so won't have problems with dangling ptr */
        IUnitView*  m_selectedUnit;
        bool m_isPerformingAction{ false };
        controllers::GameController m_gameController;

        UnitSetupView m_unitsSetupView;
        sf::Clock m_timer;

        sf::Vector2f m_mousePosBeforeMoving;
        sf::View     m_view;
        bool         m_isViewMoving{ false };
        ///float        m_currentZoom{ 1.f };
        sf::Vector2f m_startViewSize;
    };
}