#pragma once

#include "scene_node.h"
#include "../units_factory.h"
#include "../controllers/game_controller.h"
#include "board_view.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <map>

namespace graphics
{
	constexpr int kBackgroundColorR = 169;
	constexpr int kBackgroundColorG = 169;
	constexpr int kBackgroundColorB = 169;
	constexpr int kBackgroundColorAlpha = 120;
	constexpr int kTooltipCharacterSize = 120;
	constexpr int kBorderOffset = 20;
	using UnitIDMapUnit = std::map<UnitIdentifier, UnitViewPtr, Comparator<UnitIdentifier>>;

	class UnitSetupView : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
	{
	public:

		UnitSetupView();

		void  show();
		void  hide();
		void  setAvailableUnits();
		void  draw(sf::RenderWindow& target) const;
		void  draw(sf::RenderTarget& target, sf::RenderStates states) const override {}
		void  setSize(const sf::Vector2f& size);
		void  handleEvent(const sf::Event::EventType& evenType, const sf::Vector2f& mousePos, const BoardView& board);
		inline bool isUnitNewUnitPicked() const { return is_addingNewUnit; }

		//virtual ~UnitSetupView() = default;
		void setCenter(const sf::Vector2f& pos);
		void setDragDropUnitView(const sf::Vector2f& mousePos);
		inline void moveDragDropUnit(const sf::Vector2f mousePos) { (*m_unitDragDropView).setPosition(mousePos); }
		void setUnitSetupController(controllers::UnitSetupContoller controller) { m_unitSetupController = std::move(controller); }
		void resetPickedUnit();

		UnitIDMapUnit& getAddedUnitRef() { return m_addedUnits; }

	private:
		sf::RectangleShape m_background;
		std::vector<UnitViewPtr> m_availableUnits;
		SceneNodePtr m_unitDragDropView;
		UnitIDMapUnit m_addedUnits;
		controllers::UnitSetupContoller m_unitSetupController;

		TanksFactory m_tankFactory;
		bool is_addingNewUnit{ false };
		const sf::Color m_backgroundColor{ kBackgroundColorR, kBackgroundColorG, kBackgroundColorB, kBackgroundColorAlpha };
	};
}
