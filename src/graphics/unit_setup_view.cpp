#include "unit_setup_view.h"
#include "tank_view.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

#include <memory>

constexpr int kTooltipCharacterSize = 20;
constexpr int kBorderOffset = 20;

graphics::UnitSetupView::UnitSetupView() : m_textHelper("Deployment phase. To start battle press Enter", sf::Color::Black)
{
    m_background.setOutlineColor(sf::Color::Black);
    m_background.setFillColor(m_backgroundColor);
    m_background.setOutlineThickness(1);
    sf::FloatRect bounds = m_background.getLocalBounds();
    m_background.setOrigin(bounds.width / 2.f, bounds.height);
    setAvailableUnits();
}

void graphics::UnitSetupView::show()
{
    m_textHelper.setTextPosition({ m_background.getGlobalBounds().left + m_background.getSize().x - 1.25f * m_textHelper.getTextWidth(), m_background.getGlobalBounds().top });
    m_textHelper.setTextColor(sf::Color::Black);
    m_textHelper.setCharacterSize(kTooltipCharacterSize);
}

void graphics::UnitSetupView::setAvailableUnits()
{
    Angle rotation{ 90.f };
    float scalePreview = 0.2f;
    auto tmpTankView = m_tankFactory.createUnitView(core::UnitType::BasicTank, UnitIdentifier{ 0 }, scalePreview);
    tmpTankView->setRotation(rotation.angle);

    auto parentBounds = m_background.getLocalBounds();
    (*tmpTankView).setPosition(parentBounds.width / 2.f, parentBounds.height / 4.f);
    m_availableUnits.push_back(std::move(tmpTankView));
}

void graphics::UnitSetupView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_state == State::Hidden)
        return;

    target.draw(m_background, states);
    for (const auto& unit : m_availableUnits) {
        target.draw(*unit, states);
    }
    if (m_state == State::UnitPicked) {
        target.draw(*m_unitDragDropView, states);
    }

    //auto mouse_pos = sf::Mouse::getPosition(Statarget);
    //auto translated_pos = target.mapPixelToCoords(mouse_pos);
    for (const auto& [id, unit] : m_addedUnits) {
        target.draw(*unit, states);
        //unit->showTooltip(translated_pos);
    }
    target.draw(m_textHelper, states);
}

void graphics::UnitSetupView::setSize(const sf::Vector2f& size)
{
    m_background.setSize(size); 
    sf::FloatRect bounds = m_background.getLocalBounds();
    m_background.setOrigin(bounds.width / 2.f, bounds.height);
    m_textHelper.setTextPosition({ m_background.getGlobalBounds().left + m_background.getSize().x - 1.25f * m_textHelper.getTextWidth(), m_background.getGlobalBounds().top });
}

void graphics::UnitSetupView::handleEvent(const sf::Event::EventType& evenType, const sf::Vector2f& mousePos, const BoardView& board)
{
    if (m_state == State::Hidden)
        return;

    switch (evenType)
    {
    case sf::Event::MouseButtonReleased:
        if ((m_state == State::UnitPicked))
        {
            auto unitPlace = board.getCoordinatesIfValid(mousePos);
            if (unitPlace)
            {
                auto sceneNodePtr = m_unitSetupController.addUnit(*unitPlace);
                if (sceneNodePtr)
                {
                    (*sceneNodePtr)->setPosition((board).getPositionByTileCoordinates(*unitPlace));
                    m_addedUnits.insert({ (*sceneNodePtr)->getId(), std::move(*sceneNodePtr) });
                    m_textHelper.setBlinkState(BlinkingTooltip::State::Blinking);
                }

            }
            resetPickedUnit();
        }
        break;
    case sf::Event::MouseButtonPressed:
        for (const auto& unit : m_availableUnits) {
            if ((*unit).getBoundingRect().contains(mousePos)) {
                setDragDropUnitView(mousePos);
                m_state = State::UnitPicked;
            }
        }
        break;
    case sf::Event::MouseMoved:
        if (m_state == State::UnitPicked) {
            moveDragDropUnit(mousePos);
        }
        break;
    }
}

void graphics::UnitSetupView::update(sf::Time dt)
{
    if (m_state == State::Hidden)
        return;

    m_textHelper.update(dt);
}

void graphics::UnitSetupView::setCenter(const sf::Vector2f& pos)
{
    m_background.setPosition(pos);
    m_textHelper.setTextPosition({ m_background.getGlobalBounds().left + m_background.getSize().x - 1.25f * m_textHelper.getTextWidth(), m_background.getGlobalBounds().top });
    auto parentPos = m_background.getPosition();
    // now only one unit by Default
    m_availableUnits[0]->setPosition(parentPos.x, parentPos.y - (m_background.getSize().y / 2.f));
}

void graphics::UnitSetupView::setDragDropUnitView(const sf::Vector2f& mousePos)
{
    Angle rotation{ 90.f };
    float scalePreview = 0.17f;
    m_unitDragDropView = m_tankFactory.createUnitView(core::UnitType::BasicTank, UnitIdentifier{ 0 }, scalePreview);
    m_unitDragDropView->setRotation(rotation.angle);
    m_unitDragDropView->setPosition(mousePos);
}

void graphics::UnitSetupView::resetPickedUnit()
{
    m_unitDragDropView.reset();
    m_state = State::Idle;
}
