#pragma once

#include "scene_node.h"
#include "board_view.h"
#include "../src/core/data_types.h"
#include "../controllers/game_controller.h"
#include "unit_setup_view.h"

#include <map>
#include <memory>

using SceneNodePtr = std::unique_ptr<graphics::SceneNode>;

class GameBuilder;

namespace graphics {

	class GameWorldView : public events::Observer<core::GameEngine>
	{
	public:
		//GameWorldView() = default;
		GameWorldView(sf::RenderWindow& target, BoardView board, controllers::GameController controller);
		bool addNewUnitView(SceneNodePtr unit, core::GameTile position);
		void draw();
		void update(sf::Event& event);
	private:
		void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos);
		void moveView(const sf::Event& event, const sf::Vector2f& mousePos);
		bool checkIfClickedOnOwnUnit(const sf::Vector2f& mousePos);
		void onBoardClicked(const sf::Vector2f& mousePos);
		void clearMoveArea();
		void endSetupStage();

		// observer events
		void newUnitSelected(const UnitSelectedInfo& unitInfo) override;
		void informationMsgRecieved(const GameInfoMessage& msgInfo) override;
		void moveAreaRecieved(const MoveAreaInfo& moveArea) override;
		void moveUnitRecieved(const MoveUnitInfo& moveUnit) override;
		void setUnitSetupController(controllers::UnitSetupContoller controller) { m_unitsSetupView.setUnitSetupController(std::move(controller)); }
		friend class ::GameBuilder;
	private:
		sf::RenderWindow& m_renderTarget;
		BoardView m_board;
		std::map<UnitIdentifier, SceneNodePtr, Comparator<UnitIdentifier>>  m_unitsGraph{};
		PlayerIdentifier m_playerId{ 1 };
		SceneNode*  m_selectedUnit;
		controllers::GameController m_gameController;

		UnitSetupView m_unitsSetupView;
		bool m_isUnitSetupViewHided{ false };

		// TODO prob hide all view logic inside separate class
		sf::Vector2f m_mousePosBeforeMoving;
		sf::View     m_view;
		bool         m_isViewMoving{ false };
	};
}