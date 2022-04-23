#include "game_world_view.h"
#include "tank_view.h"

#include <ranges>

const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
constexpr float kTooltipAnimationTime = 3;
constexpr float kZoomViewOffset = 0.2f;

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
    m_board.draw(m_renderTarget);
    if(!m_isUnitSetupViewHided)
        m_unitsSetupView.draw(m_renderTarget);

    auto mouse_pos = sf::Mouse::getPosition(m_renderTarget);
    auto translated_pos = m_renderTarget.mapPixelToCoords(mouse_pos);
    for (const auto& [id, unit] : m_units)
    {
        m_renderTarget.draw(*unit);
        unit->showTooltip(translated_pos);
    }
    for (const auto& view : m_views)
        m_renderTarget.draw(*view);
}

void graphics::GameWorldView::update(sf::Event& event)
{
    auto mousePos = sf::Mouse::getPosition(m_renderTarget);
    auto translatedMousePos = m_renderTarget.mapPixelToCoords(mousePos);

    // actually useless anyway events are gonna be saved in poll event
    if (!m_selectedUnit->isPerformingAction())
    {
        while (m_renderTarget.pollEvent(event))
        {
            m_unitsSetupView.handleEvent(event.type, translatedMousePos, m_board);
            handleEvent(event, translatedMousePos);
        }
    }

    for (const auto& [id, unit] : m_units) unit->update(TimePerFrame);
    for (const auto& view : m_views) view->update(TimePerFrame);
}

void graphics::GameWorldView::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos)
{
    // TODO would be good to refactor this horrible switch with states or at least separates functions
    switch (event.type)
    {
    case sf::Event::MouseButtonReleased:
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (!checkIfClickedOnUnit(mousePos))
            {
                onBoardClicked(mousePos);
            }
        }
        else  {
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
        checkMousePosition(mousePos);
        moveView(event, mousePos);
        break;;
    case sf::Event::MouseWheelMoved:
        resizeView((event.mouseWheelScroll.wheel > 0.f) ? -kZoomViewOffset : kZoomViewOffset);
        break;
    case sf::Event::KeyPressed:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            m_gameController.finishSetupStage(m_playerId);
            m_units.merge(m_unitsSetupView.getAddedUnitRef());
            endSetupStage();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) // TODO only for tests of swtiching action phase
        {
            m_selectedUnit->markAsSelected();
            clearMoveArea();
            m_selectedUnit = m_units[UnitIdentifier{ 0 }].get();
            m_gameController.finishActionPhase(m_playerId);
            std::erase_if(m_views,  [](const auto& value) {return value->isMarkedToDelete() == true; });
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

void graphics::GameWorldView::resizeView(const float resizeFactor)
{
    m_view.zoom(1.f + resizeFactor);
    m_renderTarget.setView(m_view);
    auto [width, height] = m_view.getSize();
    auto viewCenter = m_view.getCenter();
    m_unitsSetupView.setCenter(sf::Vector2f{ viewCenter.x, viewCenter.y + height / 2.f });
}

bool graphics::GameWorldView::checkIfClickedOnUnit(const sf::Vector2f& mousePos)
{
    for (const auto& [id, unit] : m_units) 
    {
        if (unit->getBoundingRect().contains(mousePos)) 
        {
            if (m_gameController.isEnemyUnitId(unit->getId()))
            {
                auto tileCenter = m_board.getTileCenterIfValid(mousePos);
                if (!tileCenter) {
                    std::cout << "Unit: " << unit->getId() << " has invalid position\n";
                    return false; // this means unit outside of the board maybe throw?
                }
                Angle gunRotation = m_selectedUnit->calculateGunRotation(m_selectedUnit->getPosition(), *tileCenter);
                m_gameController.onChangeUnitRotation(m_selectedUnit->getId(), gunRotation, RotateUnitActiom::Type::Gun);
            }
            m_gameController.onUnitClicked(m_selectedUnit->getId(), unit->getId());
            return true;
        }
    }
    return false;
}

void graphics::GameWorldView::checkMousePosition(const sf::Vector2f& mousePos)
{
    for (const auto& [id, unit] : m_units)
    {
        if (unit->getBoundingRect().contains(mousePos))
        {
            Angle requiredGunAngleToShot{ 0.0f };
            if(m_selectedUnit->getId())
                requiredGunAngleToShot = m_selectedUnit->calculateGunRotation(m_selectedUnit->getPosition(), unit->getPosition());
            m_gameController.onShowUnitStateMsg(m_selectedUnit->getId(), unit->getId(), requiredGunAngleToShot);
        }
    }
}

void graphics::GameWorldView::onBoardClicked(const sf::Vector2f& mousePos)
{
    m_gameController.moveUnit(m_selectedUnit->getId(), (m_board).getSelectorTileCoordinates());
}

void graphics::GameWorldView::newUnitSelected(const UnitSelectedInfo& unitInfo)
{
    if (unitInfo.m_unitId == m_selectedUnit->getId())
        return;

    if (m_units.contains(unitInfo.m_unitId))
    {
        m_selectedUnit->markAsSelected();
        //m_selectedUnit = m_units[UnitIdentifier{ 0 }].get();
        clearMoveArea();

        m_selectedUnit = m_units[unitInfo.m_unitId].get();
        m_selectedUnit->markAsSelected();
    }
}

void graphics::GameWorldView::informationMsgRecieved(const core::UnitStateMsg& unitStateMsg)
{
    std::vector<tankDamageSystem::DamageTo> damageToVec{};
    if (unitStateMsg.m_hiddenDamageCounter)
        damageToVec.push_back(core::kHiddenDamage);
    for (const auto& crew : unitStateMsg.m_crewInfo)
        if (crew.m_state == tankDamageSystem::DamageStatus::Damaged) damageToVec.push_back(crew.m_name);
    for (const auto& part : unitStateMsg.m_unitParts)
        if(part.m_state == tankDamageSystem::DamageStatus::Damaged) damageToVec.push_back(part.m_name);

    std::vector<std::string> unitInfo{}; 
    if (unitStateMsg.m_pointOfView == PointOfView::Player)
    {
        unitInfo.push_back(std::to_string(unitStateMsg.m_actionState.m_remainingMovePoints.distance) + "/" + std::to_string(unitStateMsg.m_actionState.m_fullMovePoints.distance));
        unitInfo.push_back(std::to_string(unitStateMsg.m_actionState.m_remainingShots.shots) + "/" + std::to_string(unitStateMsg.m_actionState.m_rateOfFire.shots));
    }
    else
    {
        unitInfo.push_back("??/??");
        unitInfo.push_back("??/??");
    }
    unitInfo.push_back(std::to_string(unitStateMsg.m_hiddenDamageCounter));
    m_units[unitStateMsg.m_id]->setTooltipMsg(unitStateMsg.m_isAlive, unitInfo, damageToVec);
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
    //    for(const auto& unit: m_units)
    //        if (moveUnit.m_unitId == m_selectedUnit->getId())

    core::GameTile dest = moveUnit.m_unitPos;
    if (size(moveUnit.m_movePath)) dest = moveUnit.m_movePath.back();
    auto rotationPoint = m_board.getTileVertex(dest, moveUnit.m_rotation);
    m_units[moveUnit.m_unitId]->setUpRotationAnimation(m_units[moveUnit.m_unitId]->getPosition(), rotationPoint);
    m_units[moveUnit.m_unitId]->setCurrentRotationPoint(rotationPoint);

    auto movementPath = m_board.convertTileCoordinatesToScreenPos(moveUnit.m_movePath);
    if (size(movementPath) == 0) return;

    m_units[moveUnit.m_unitId]->setMovementPath(std::move(movementPath));
    //m_isPerformingAction = true;
}

void graphics::GameWorldView::ChangeUnitStateRecieved(const UnitStateInfo& unitState)
{
    auto animatedTooltip = std::make_unique<AnimatedTooltip>(m_unitsSetupView.getTextureHolder(), kTooltipAnimationTime);
    animatedTooltip->setText(std::string(unitState.m_damageTypeName), m_units[unitState.m_targetUnit]->getPosition());
    m_views.push_back(std::move(animatedTooltip));
    
    Angle requiredGunAngleToShot = m_selectedUnit->calculateGunRotation(m_units[unitState.m_srcUnit]->getPosition(), m_units[unitState.m_targetUnit]->getPosition());
    m_gameController.onShowUnitStateMsg(unitState.m_srcUnit, unitState.m_targetUnit, requiredGunAngleToShot);

    if (unitState.m_damageTypeName == core::kShotMissed)
        return;

    if (unitState.m_damageStatus == core::DamageStatus::Damaged)
        if (unitState.m_srcUnit != UnitIdentifier{ 0 })
            m_units[unitState.m_srcUnit]->shot(m_units[unitState.m_targetUnit].get(), unitState.m_damageTypeName);
        else
            m_units[unitState.m_targetUnit]->showDamage(unitState.m_damageTypeName);
    else
        m_units[unitState.m_targetUnit]->recoveryDamage(unitState.m_damageTypeName);
}

void graphics::GameWorldView::rotateGunRecieved(const RotateGunInfo& rotateUnitGun)
{
    m_units[rotateUnitGun.m_unitId]->setUpGunRotationAnimation(rotateUnitGun.m_gunRotation);
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

bool graphics::GameWorldView::addNewUnitView(UnitViewPtr unit, core::GameTile position)
{
    unit->setPosition(m_board.getPositionByTileCoordinates(position));
    m_units.insert({ unit->getId(), std::move(unit) });
    return true;
}

void graphics::GameWorldView::addNullUnit(UnitViewPtr nullUnit)
{
    m_units.insert({ UnitIdentifier{ 0 }, std::move(nullUnit) });
    m_selectedUnit = m_units[UnitIdentifier{ 0 }].get();
}
