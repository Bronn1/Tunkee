#include "unit_setup_view.h"
#include "tank_view.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

#include <memory>

graphics::UnitSetupView::UnitSetupView()
{
    m_background.setOutlineColor(sf::Color::Black);
    m_background.setFillColor(m_backgroundColor);
    m_background.setOutlineThickness(1);
    sf::FloatRect bounds = m_background.getLocalBounds();
    m_background.setOrigin(bounds.width / 2.f, bounds.height);


    setAvailableUnits();

}

void graphics::UnitSetupView::setAvailableUnits()
{
    Angle rotation{ 90.f };
    float scalePreview = 0.2f;
    auto tmpTankView = m_tankFactory.createBacisTankView(UnitIdentifier{ 0 }, rotation, scalePreview);

    auto parentBounds = m_background.getLocalBounds();
    (*tmpTankView).setPosition(parentBounds.width / 2.f, parentBounds.height / 4.f);
    m_availableUnits.push_back(std::move(tmpTankView));
}

void graphics::UnitSetupView::draw(sf::RenderWindow& target) const
{
    target.draw(m_background);
    for (const auto& unit : m_availableUnits)
    {
        target.draw(*unit);
    }
    if (isUnitNewUnitPicked())
    {
        target.draw(*m_unitDragDropView);
    }

    auto mouse_pos = sf::Mouse::getPosition(target);
    auto translated_pos = target.mapPixelToCoords(mouse_pos);
    for (const auto& [id, unit] : m_addedUnits)
    {
        target.draw(*unit);
        unit->showTooltip(translated_pos);
    }
}

void graphics::UnitSetupView::setSize(const sf::Vector2f& size)
{
    m_background.setSize(size); 
    sf::FloatRect bounds = m_background.getLocalBounds();
    m_background.setOrigin(bounds.width / 2.f, bounds.height);
}

void graphics::UnitSetupView::handleEvent(const sf::Event::EventType& evenType, const sf::Vector2f& mousePos, const BoardView& board)
{
    switch (evenType)
    {
    case sf::Event::MouseButtonReleased:
        if (isUnitNewUnitPicked())
        {
            auto unitPlace = board.getCoordinatesIfValid(mousePos);
            if (unitPlace)
            {
                auto sceneNodePtr = m_unitSetupController.addUnit(*unitPlace);
                if (sceneNodePtr)
                {
                    (*sceneNodePtr)->setPosition((board).getPositionByTileCoordinates(*unitPlace));
                    m_addedUnits.insert({ (*sceneNodePtr)->getId(), std::move(*sceneNodePtr) });
                }

            }
            resetPickedUnit();
        }
        break;
    case sf::Event::MouseButtonPressed:

        for (const auto& unit : m_availableUnits)
        {
            if ((*unit).getBoundingRect().contains(mousePos))
            {
                setDragDropUnitView(mousePos);
                is_addingNewUnit = true;
            }
        }
       
        break;
    case sf::Event::MouseMoved:
    {
        if (isUnitNewUnitPicked())
        {
            moveDragDropUnit(mousePos);
        }
        break;
    }
    }
}

void graphics::UnitSetupView::setCenter(const sf::Vector2f& pos)
{
    m_background.setPosition(pos);
    auto parentPos = m_background.getPosition();
    // damn..
    m_availableUnits.back()->setPosition(parentPos.x, parentPos.y - (m_background.getSize().y / 2.f));
}

void graphics::UnitSetupView::setDragDropUnitView(const sf::Vector2f& mousePos)
{
    Angle rotation{ 90.f };
    float scalePreview = 0.17f;
    m_unitDragDropView = m_tankFactory.createBacisTankView(UnitIdentifier{ 0 }, rotation, scalePreview);
    m_unitDragDropView->setPosition(mousePos);
}

void graphics::UnitSetupView::hide()
{
}

void graphics::UnitSetupView::resetPickedUnit()
{
    m_unitDragDropView.reset();
    is_addingNewUnit = false;
}
