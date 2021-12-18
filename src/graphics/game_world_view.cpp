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

    auto nullUnit = std::make_unique<NullUnitView>();
    m_unitsGraph.insert({ UnitIdentifier{ 0 }, std::move(nullUnit) });
    m_selectedUnit = m_unitsGraph[UnitIdentifier{ 0 }].get();
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
        (*unit).showTooltip(translated_pos);
    }
}

void graphics::GameWorldView::update(sf::Event& event)
{
    auto mousePos = sf::Mouse::getPosition(m_renderTarget);
    auto translatedMousePos = m_renderTarget.mapPixelToCoords(mousePos);

    while (m_renderTarget.pollEvent(event))
    {
        m_unitsSetupView.handleEvent(event.type, translatedMousePos, m_board);
        handleEvent(event, translatedMousePos);
    }
}

void graphics::GameWorldView::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos)
{
    switch (event.type)
    {
    case sf::Event::MouseButtonReleased:
        if (!checkIfClickedOnOwnUnit(mousePos))
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
            m_selectedUnit->drawAsSelected();
            clearMoveArea();
            m_selectedUnit = m_unitsGraph[UnitIdentifier{ 0 }].get();
            m_gameController.finishActionPhase(m_playerId);
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

bool graphics::GameWorldView::checkIfClickedOnOwnUnit(const sf::Vector2f& mousePos)
{
    for (const auto& [id, unit] : m_unitsGraph) 
    {
        if ((*unit).getBoundingRect().contains(mousePos)) 
        {
            m_selectedUnit->drawAsSelected();
            m_selectedUnit = m_unitsGraph[UnitIdentifier{ 0 }].get();
            clearMoveArea();
            m_gameController.onUnitClicked((*unit).getId());
        }
    }

    return false;
}

void graphics::GameWorldView::onBoardClicked(const sf::Vector2f& mousePos)
{
    auto unit = m_selectedUnit;
    m_gameController.moveUnit(m_selectedUnit->getId(), (m_board).getSelectorTileCoordinates());
    //m_selectedUnit->rotateTurretTo(m_selectedUnit->getPosition(), mousePos);
}

void graphics::GameWorldView::newUnitSelected(const UnitSelectedInfo& unitInfo)
{
    if (m_unitsGraph.contains(unitInfo.m_unitId))
    {
        m_selectedUnit = m_unitsGraph[unitInfo.m_unitId].get();
        m_unitsGraph[unitInfo.m_unitId]->drawAsSelected();
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
    m_unitsGraph[moveUnit.m_unitId]->drawAsSelected();
    float prevRotation = m_unitsGraph[moveUnit.m_unitId]->getRotation();
    auto movementVector = m_board.getVectorPositionsByTiles(moveUnit.m_movePath);
    for (const auto& targetPoint : movementVector)
    {
        auto curPoint = m_unitsGraph[moveUnit.m_unitId]->getPosition();
        m_unitsGraph[moveUnit.m_unitId]->rotateTo(curPoint, targetPoint);

        sf::Clock clock;
        sf::Time timer = clock.getElapsedTime();
        while (timer.asMilliseconds() <= 1000)
        {
            timer = clock.getElapsedTime();
            float  speed  = timer.asMilliseconds() / 1000.f;
            m_unitsGraph[moveUnit.m_unitId]->setPosition(m_unitsGraph[moveUnit.m_unitId]->interpolateWithFactor(curPoint, targetPoint, speed));
            draw();
            m_renderTarget.display();
        }
    }
    m_unitsGraph[moveUnit.m_unitId]->setRotation(prevRotation);
    m_unitsGraph[moveUnit.m_unitId]->drawAsSelected();
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
bool graphics::GameWorldView::addNewUnitView(SceneNodePtr unit, core::GameTile position)
{
    //auto is_tile_exists = [&position](TileView tile) { return  tile.getCoordinates() == position; };
    //if (auto result = std::ranges::find_if(m_tiles, is_tile_exists); result != m_tiles.end())
    m_gameController.addOwnUnit((*unit).getId());
    unit->setPosition((m_board).getPositionByTileCoordinates(position));
     m_unitsGraph.insert({ (*unit).getId(), std::move(unit) });
    return true;
}
