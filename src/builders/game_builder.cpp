#include "game_builder.h"
#include "../controllers/movement_controller.h"

//TODO Unit for tests only!
class MockUnit : public core::Unit {
public:
	MockUnit(ObjectIdentifier id, TileDistance dis) : Unit(id, TileDistance{ dis }, Shots{ 3 }) {}

};

GameBuilder::GameBuilder()
{
}

void GameBuilder::reset()
{
	//m_boardModel.reset();
	//m_boardView.reset();
}

GameBuilder GameBuilder::createMinimalGame(sf::RenderWindow& target, sf::Texture* texture)
{
	GameBuilder builder;
	//reset();
	std::vector<core::GameTileType> tileset = builder.generateMinimalTileset();

	int boardSize = std::size(tileset);
	auto boardModel = core::GameBoard(tileset, boardSize, boardSize);
	auto boardView  = graphics::BoardView(boardModel.getTileCoordinates(), texture, boardSize);

	auto unitMgr = std::make_shared<core::UnitManager>();
	std::shared_ptr<core::Unit> unit = std::make_shared<MockUnit>(ObjectIdentifier{ 1 }, TileDistance{ 7 });
	unit.get()->setPosition(core::GameTile(0, 0));
	unit.get()->setOwner(PlayerIdentifier{ 1 });
	(*unitMgr).addUnit(unit);

	builder.m_gameEngineRules = std::make_unique< core::GameRulesEngine>(boardModel, std::move(unitMgr), PlayerIdentifier{1 }, PlayerIdentifier{2 });
	auto movementController = controllers::MovementController(builder.m_gameEngineRules.get(), PlayerIdentifier{ 1 });
	builder.m_worldView = std::make_unique< graphics::GameWorldView>(target, boardView, std::move(movementController));
	return builder;
}
