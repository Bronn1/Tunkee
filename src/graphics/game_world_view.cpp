#include "game_world_view.h"

#include <ranges>

const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

graphics::GameWorldView::GameWorldView(sf::RenderWindow& target, BoardViewPtr board, controllers::MovementController controller) :
	m_renderTarget(target), m_board(std::move(board)), m_movementConroller(controller)
{
}

void graphics::GameWorldView::draw()
{
    (m_board).draw(m_renderTarget);
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
        switch (event.type)
        {
            case sf::Event::MouseButtonReleased:
                if (!selectNewUnit(translatedMousePos) && m_unitsGraph.contains(m_selectedUnitId))
                {
                    moveSelectedUnit(translatedMousePos);
                }
                break;
            case sf::Event::MouseButtonPressed:
                break;

            case sf::Event::Closed:
                m_renderTarget.close();
                break;
                
                // check the type of the event... event.type == sf::Event::MouseButtonPressed
            }
        }
}

bool graphics::GameWorldView::selectNewUnit(const sf::Vector2f& mousePos)
{
    for (const auto& [id, unit] : m_unitsGraph)
    {
        if ((*unit).getBoundingRect().contains(mousePos) && m_selectedUnitId != id)
        {
            if (m_unitsGraph.contains(m_selectedUnitId)) {
                auto prevSelectedUnit = m_unitsGraph[m_selectedUnitId].get();
                prevSelectedUnit->setAsSelected();
            }
            (*unit).setAsSelected();
            m_selectedUnitId = id;
            return true;
        }

    }

    return false;
}

void graphics::GameWorldView::moveSelectedUnit(const sf::Vector2f& mousePos)
{

    for (const auto& [id, unit] : m_unitsGraph)
    {
        auto unit = m_unitsGraph[m_selectedUnitId].get();
        sf::Vector2f curPos = unit->getPosition();

        // TODO Should be in Unit class
        // now we have both the sprite position and the cursor
        // position lets do the calculation so our sprite will
        // face the position of the mouse
        const float PI = 3.14159265;

        float dx = curPos.x - mousePos.x;
        float dy = curPos.y - mousePos.y;

        float turretTextureOffset = 90;
        float rotation = ((atan2(dy, dx)) * 180 / PI) + turretTextureOffset;
        auto movePath = m_movementConroller.moveUnit(m_selectedUnitId, (m_board).getTileSelectorCoordinates());
        if (std::size(movePath))
        {
            std::cout << movePath.back() << " Moved to\n";
            unit->setPosition((m_board).getPositionByTileCoordinates(movePath.back()));
        }
        unit->rotateTurretTo(rotation);
        break;
    }
}


bool graphics::GameWorldView::addNewUnitView(SceneNodePtr unit, core::GameTile position)
{
    //auto is_tile_exists = [&position](TileView tile) { return  tile.getCoordinates() == position; };
    //if (auto result = std::ranges::find_if(m_tiles, is_tile_exists); result != m_tiles.end())
    unit->setPosition((m_board).getPositionByTileCoordinates(core::GameTile(0,0)));
     m_unitsGraph.insert({ (*unit).getId(), std::move(unit) });
    return true;
}
