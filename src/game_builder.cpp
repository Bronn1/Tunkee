#include "game_builder.h"
#include "controllers/game_controller.h"
#include "core/unit.h"
#include "core/player.h"

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

	auto unitMgr = std::make_unique<core::UnitManager>();
	builder.m_gameEngine = std::make_unique< core::GameEngine>(boardModel, std::move(unitMgr), PlayerIdentifier{1 }, PlayerIdentifier{2 });
	core::Player playerOne({ 1 });
	// TODO builder should have ability to set player later in case of multiplayer game
	core::Player playerTwo({ 2 });
	builder.m_gameEngine.get()->setPlayer(playerOne);
	builder.m_gameEngine.get()->setPlayer(playerTwo);

	auto movementController = controllers::GameController(builder.m_gameEngine.get(), PlayerIdentifier{ 1 });
	auto unitCreatorController = controllers::UnitSetupContoller(builder.m_gameEngine.get(), PlayerIdentifier{ 1 });
	builder.m_worldView = std::make_unique< graphics::GameWorldView>(target, boardView, std::move(movementController));
	builder.m_worldView.get()->setUnitSetupController(std::move(unitCreatorController));
	builder.m_gameEngine.get()->subscribe(builder.m_worldView.get());
	//builder.m_gameEngine.get()->setUpdateViewInterface(builder.m_worldView.get());
	return builder;
}

GameBuilder& GameBuilder::addAi()
{
	// TODO we will create some enemy units for test purposes, later on all units should be created inside ai module
	// depending on picked mission, compain and etc
	auto firstUnitPos = core::GameTile(0, 8);
	auto secondUnitPos = core::GameTile(3, 8);
	auto thirdUnitPos = core::GameTile(6, 8);
	PlayerIdentifier player{ 2 };
    auto tmpUnit1 = m_tankFactory.createBacisTank(firstUnitPos, player);
	auto tmpUnit2 = m_tankFactory.createBacisTank(secondUnitPos, player);
	auto tmpUnit3 = m_tankFactory.createBacisTank(thirdUnitPos, player);
	//UnitIdentifier unitId = (*tmpUnit).getID();
	UnitIdentifier addedUnitID = m_gameEngine->addNewUnit(std::move(tmpUnit1));
	//UnitIdentifier addedUnitID2 = m_gameEngine->addNewUnit(std::move(tmpUnit2));
	//UnitIdentifier addedUnitID3 = m_gameEngine->addNewUnit(std::move(tmpUnit3));

	float rotation = 270.f;
	float scale = 0.17f;
	m_worldView.get()->addNewUnitView(std::move(m_tankFactory.createBacisTankView(addedUnitID, rotation, scale)), firstUnitPos);
	//m_worldView.get()->addNewUnitView(std::move(m_tankFactory.createBacisTankView(addedUnitID2, rotation, scale)), secondUnitPos);
	//m_worldView.get()->addNewUnitView(std::move(m_tankFactory.createBacisTankView(addedUnitID3, rotation, scale)), thirdUnitPos);

	return *this;
}
