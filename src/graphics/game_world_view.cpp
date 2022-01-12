#include "game_world_view.h"
#include "tank_view.h"

#include <ranges>

const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

graphics::GameWorldView::GameWorldView(sf::RenderWindow& target, BoardView board, controllers::GameController controller) :
	m_renderTarget(target), m_board(std::move(board)), m_gameController(controller)
{
    m_view = target.getDefaultView();
    auto [height, width] = m_renderTarget.getSize();
    m_unitsSetupView.setSize(sf::Vector2f{ (float)height, (float)width / 4.f });
    auto [view_height, view_width] = m_view.getSize();
    m_unitsSetupView.setCenter(sf::Vector2f{ (float)view_height / 2.f, (float)view_width });
}

void graphics::GameWorldView::draw()
{
    (m_board).draw(m_renderTarget);
    if(!m_isUnitSetupViewHided)
        m_unitsSetupView.draw(m_renderTarget);

    auto mouse_pos = sf::Mouse::getPosition(m_renderTarget);
    auto translated_pos = m_renderTarget.mapPixelToCoords(mouse_pos);
    for (const auto& [id, unit] : m_unitsGraph)
    {
        m_renderTarget.draw(*unit);
        unit->showTooltip(translated_pos);
    }
    for (const auto& projectile : m_projectiles)
        m_renderTarget.draw(*projectile);
}

void graphics::GameWorldView::update(sf::Event& event)
{
    auto mousePos = sf::Mouse::getPosition(m_renderTarget);
    auto translatedMousePos = m_renderTarget.mapPixelToCoords(mousePos);

    // TODO actually useless anyway events are gonna be saved in poll event
    if (!m_selectedUnit->isPerformingAction())
    {
        while (m_renderTarget.pollEvent(event))
        {
            m_unitsSetupView.handleEvent(event.type, translatedMousePos, m_board);
            handleEvent(event, translatedMousePos);
        }
    }
    if (!m_selectedUnit->isPerformingAction() && m_isPerformingAction)
    {
        m_isPerformingAction = false;
        m_gameController.onChangeUnitRotation(m_selectedUnit->getId(), Angle{ m_selectedUnit->getRotation() }, SetUnitRotation::Type::Body);
    }

    for (const auto& [id, unit] : m_unitsGraph)
    {
        unit->update(TimePerFrame);
    }
    for (const auto& projectile : m_projectiles)
    {
        projectile->update(TimePerFrame);
    }
}

void graphics::GameWorldView::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos)
{
    switch (event.type)
    {
    case sf::Event::MouseButtonReleased:
        if (!checkIfClickedOnUnit(mousePos))
        {
            onBoardClicked(mousePos);
        }
        if (event.mouseButton.button == sf::Mouse::Right) {
            m_isViewMoving = false;
        }
        break;
    case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Right) {
            m_isViewMoving = true;
            m_mousePosBeforeMoving = m_renderTarget.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        }
        break;
    case sf::Event::MouseMoved:
    {
        moveView(event, mousePos);
        break;;
    }
    case sf::Event::KeyPressed:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            m_gameController.finishSetupStage(m_playerId);
            m_unitsGraph.merge(m_unitsSetupView.getAddedUnitRef());
            endSetupStage();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            // TODO only for tests of swtiching action phase
            m_selectedUnit->markAsSelected();
            clearMoveArea();
            m_selectedUnit = m_unitsGraph[UnitIdentifier{ 0 }].get();
            m_gameController.finishActionPhase(m_playerId);
            std::erase_if(m_projectiles,  [](const auto& value) {return value->isDestoyed() == true; });
        }
        break;
    case sf::Event::Closed:
        m_renderTarget.close();
        break;
    case sf::Event::Resized:
        auto [height, width] = m_renderTarget.getSize();
        m_unitsSetupView.setSize(sf::Vector2f{ (float)height, (float)width / 4.f });
        break;
    }
}

void graphics::GameWorldView::moveView(const sf::Event& event, const sf::Vector2f& mousePos)
{
    if (m_isViewMoving)
    {
        sf::Vector2f deltaMousePos = m_mousePosBeforeMoving - mousePos;

        m_view.move(deltaMousePos);
        auto viewSize = m_view.getSize();
        auto viewCenter = m_view.getCenter();
        m_unitsSetupView.setCenter(sf::Vector2f{ viewCenter.x, viewCenter.y + viewSize.y / 2.f });
        m_renderTarget.setView(m_view);
        m_mousePosBeforeMoving = m_renderTarget.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
    }
}

bool graphics::GameWorldView::checkIfClickedOnUnit(const sf::Vector2f& mousePos)
{
    for (const auto& [id, unit] : m_unitsGraph) 
    {
        if (unit->getBoundingRect().contains(mousePos)) 
        {
            if (m_selectedUnit->getId() != UnitIdentifier{ 0 })
            {
                m_projectiles.push_back(m_selectedUnit->shot(mousePos));
            }

            m_gameController.onUnitClicked(m_selectedUnit->getId(), unit->getId());
            //unit->markAsDestoyed();
            //return true;
        }
    }

    return false;
}

void graphics::GameWorldView::onBoardClicked(const sf::Vector2f& mousePos)
{
    //auto unit = m_selectedUnit;
    m_gameController.moveUnit(m_selectedUnit->getId(), (m_board).getSelectorTileCoordinates());

    auto tileCenter =  m_board.getTileCenterIfValid(mousePos);
    if (tileCenter)
    {
        Angle gunRotation = m_selectedUnit->rotateGunTo(m_selectedUnit->getPosition(), tileCenter.value());
        m_gameController.onChangeUnitRotation(m_selectedUnit->getId(), gunRotation, SetUnitRotation::Type::Gun);
    }
}

void graphics::GameWorldView::newUnitSelected(const UnitSelectedInfo& unitInfo)
{
    if (unitInfo.m_unitId == m_selectedUnit->getId())
        return;

    if (m_unitsGraph.contains(unitInfo.m_unitId))
    {
        m_selectedUnit->markAsSelected();
        //m_selectedUnit = m_unitsGraph[UnitIdentifier{ 0 }].get();
        clearMoveArea();

        m_selectedUnit = m_unitsGraph[unitInfo.m_unitId].get();
        m_selectedUnit->markAsSelected();
    }
}

void graphics::GameWorldView::informationMsgRecieved(const GameInfoMessage& msgInfo)
{
}

void graphics::GameWorldView::moveAreaRecieved(const MoveAreaInfo& moveArea)
{
    m_board.resetMoveArea(moveArea.moveArea, moveArea.firstLayerSize);
}

void graphics::GameWorldView::moveUnitRecieved(const MoveUnitInfo& moveUnit)
{

    assert(moveUnit.m_unitId == m_selectedUnit->getId());
    // TODO how to make sure player havent picked other unit during waiting response from server with high ping??
    // in case of multiplayer game if player clicked on other unit;
    //if(moveUnit.m_unitId != m_selectedUnit->getId())
    //    for(const auto& unit: m_unitsGraph)
    //        if (moveUnit.m_unitId == m_selectedUnit->getId())
    auto movementPath = m_board.getBulkPositionsByTiles(moveUnit.m_movePath);
    if (size(movementPath) == 0)
        return;
    m_unitsGraph[moveUnit.m_unitId]->setMovementState(std::move(movementPath));
    m_isPerformingAction = true;
}

void graphics::GameWorldView::shotUnitRecieved(const ShootUnitInfo& shotUnit)
{
}

void graphics::GameWorldView::clearMoveArea()
{
    auto emptyArea = MoveAreaInfo{};
    m_board.resetMoveArea({}, 0);
}

void graphics::GameWorldView::endSetupStage()
{
    m_isUnitSetupViewHided = true;
}

/*unused for now*/
bool graphics::GameWorldView::addNewUnitView(UnitViewPtr unit, core::GameTile position)
{
    //auto is_tile_exists = [&position](TileView tile) { return  tile.getCoordinates() == position; };
    //if (auto result = std::ranges::find_if(m_tiles, is_tile_exists); result != m_tiles.end())
    //m_gameController.addOwnUnit(unit->getId());
    unit->setPosition((m_board).getPositionByTileCoordinates(position));
     m_unitsGraph.insert({ unit->getId(), std::move(unit) });
    return true;
}

void graphics::GameWorldView::addNullUnit(UnitViewPtr nullUnit)
{
    m_unitsGraph.insert({ UnitIdentifier{ 0 }, std::move(nullUnit) });
    m_selectedUnit = m_unitsGraph[UnitIdentifier{ 0 }].get();
}
