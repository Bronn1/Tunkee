#pragma once

//#include "scene_node.h"
#include "interface_unit_view.h"
#include "src/units_factory.h"
#include "src/controllers/game_controller.h"
#include "board_view.h"
#include "game_tooltip.h"

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

    using UnitViewPtr = std::unique_ptr < graphics::IUnitView>;
    using UnitIDMapUnit = std::map<UnitIdentifier, UnitViewPtr, Comparator<UnitIdentifier>>;

    // TODO standardize with ISceneNode interface
    class UnitSetupView : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
    {
    public:
        enum class State
        {
            Idle,
            UnitPicked,
            Hidden
        };
        UnitSetupView();
        void  show();
        void  setAvailableUnits();

        void  draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void  setSize(const sf::Vector2f& size);
        void  handleEvent(const sf::Event::EventType& evenType, const sf::Vector2f& mousePos, const BoardView& board);
        inline void  setState(const State state) { m_state = state; }
        void update(sf::Time dt);
        //inline bool isUnitNewUnitPicked() const { return is_addingNewUnit; }

        //virtual ~UnitSetupView() = default;
        void setCenter(const sf::Vector2f& pos);
        void setDragDropUnitView(const sf::Vector2f& mousePos);
        inline void moveDragDropUnit(const sf::Vector2f& mousePos) { m_unitDragDropView->setPosition(mousePos); }
        void setUnitSetupController(controllers::UnitSetupContoller controller) { m_unitSetupController = std::move(controller); }
        void resetPickedUnit();
        const TextureHolder& getTextureHolder() const { return m_tankFactory.getTexureHolder(); }

        UnitIDMapUnit& getAddedUnitRef() { return m_addedUnits; }

    private:
        sf::RectangleShape m_background;
        std::vector<UnitViewPtr> m_availableUnits;
        BlinkingTooltip m_textHelper;
        UnitViewPtr m_unitDragDropView;
        UnitIDMapUnit m_addedUnits;
        controllers::UnitSetupContoller m_unitSetupController;
        State m_state{ State::Idle };

        TanksFactory m_tankFactory;
        const sf::Color m_backgroundColor{ kBackgroundColorR, kBackgroundColorG, kBackgroundColorB, kBackgroundColorAlpha };
    };
}
