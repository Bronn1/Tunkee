#include "game_builder.h"
#include "controllers/game_controller.h"
#include "core/unit.h"
#include "core/player.h"

constexpr int kAlphaTestMapSize = 30;

std::vector<core::GameTileType> GameBuilder::generateMinimalTileset() const
{
    return std::vector<core::GameTileType>(kAlphaTestMapSize);
}

void GameBuilder::loadTileTextures()
{
    using enum textures::ID;
    m_tileTextures.load(TileGrass, "data/textures/grass-test1.jpg");
}

GameBuilder GameBuilder::initGameEngine(sf::RenderWindow& target)
{
    GameBuilder builder;
    std::vector<core::GameTileType> tileset = builder.generateMinimalTileset();
    builder.loadTileTextures();

    int boardSize = std::size(tileset);
    auto boardModel = core::GameBoard(tileset, boardSize, boardSize);
    auto boardView  = graphics::BoardView(boardModel.getTileCoordinates(), builder.m_tileTextures, boardSize);

    auto unitMgr = std::make_unique<core::UnitManager>();
    core::Player playerOne({ 1 });
    core::Player playerTwo({ 2 });
    builder.m_gameEngine = std::make_unique< core::GameEngine>(boardModel, std::move(unitMgr), playerOne.getId(), playerTwo.getId());
    builder.m_gameEngine.get()->setPlayer(playerOne);
    builder.m_gameEngine.get()->setPlayer(playerTwo);

    auto movementController = controllers::GameController(builder.m_gameEngine.get(), PlayerIdentifier{ 1 });
    auto unitCreatorController = controllers::UnitSetupContoller(builder.m_gameEngine.get(), PlayerIdentifier{ 1 });
    builder.m_worldView = std::make_unique< graphics::GameWorldView>(target, boardView, std::move(movementController));
    builder.m_worldView->setUnitSetupController(std::move(unitCreatorController));
    builder.m_gameEngine->subscribe(builder.m_worldView.get());
    builder.m_worldView->addNullUnit(builder.m_tankFactory.createNullUnitView());
    //builder.m_gameEngine.get()->setUpdateViewInterface(builder.m_worldView.get());
    return builder;
}

GameBuilder& GameBuilder::singleGameMode()
{
    // for alpha version just add one enemy unit to test graphcial things( burning, tooltips, etc...)
    auto firstUnitPos = core::GameTile(0, 8);
    PlayerIdentifier player{ 2 };
    Angle rotation{ 270.f };
    auto tmpUnit1 = m_tankFactory.createUnitModel(core::UnitType::BasicTank, player);
    tmpUnit1->setPosition(firstUnitPos);
    tmpUnit1->setGunRotation(rotation);
    UnitIdentifier addedUnitID = m_gameEngine->addNewUnit(std::move(tmpUnit1));
    float scale = 0.17f;
    auto tmpUnitView = m_tankFactory.createUnitView(core::UnitType::BasicTank, addedUnitID, scale);
    tmpUnitView->setRotation(rotation.angle);
    m_worldView->addNewUnitView(std::move(tmpUnitView), firstUnitPos);

    return *this;
}
