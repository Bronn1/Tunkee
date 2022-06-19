#include "game_builder.h"
#include "controllers/game_controller.h"
#include "core/unit.h"
#include "core/player.h"

constexpr int kAlphaTestMapSize = 30;
constexpr std::string_view kBoardTileTexture = "data/textures/grass-test1.jpg";

std::vector<core::GameTileType> GameBuilder::generateMinimalTileset() const // as long as map generator isn't ready
{
    return std::vector<core::GameTileType>(kAlphaTestMapSize, core::GameTileType::Grass);
}

void GameBuilder::loadTileTextures()
{
    using enum textures::ID;
    m_tileTextures.load(TileGrass, kBoardTileTexture);
}

GameBuilder GameBuilder::initGameEngine(sf::RenderWindow& target, const settings::LaunchSettings& launchSettings)
{
    GameBuilder builder;
    std::vector<core::GameTileType> tileset = builder.generateMinimalTileset();
    builder.launchSettings = launchSettings;
    builder.loadTileTextures();

    int boardSize = std::size(tileset);
    auto boardModel = core::GameBoard(tileset, boardSize, boardSize);
    boardModel.initSetupArea(launchSettings.m_host, launchSettings.setupArea);
    boardModel.initSetupArea(launchSettings.m_guest, launchSettings.setupArea);
    auto boardView  = graphics::BoardView(boardModel.getTileCoordinates(), builder.m_tileTextures, boardSize);

    auto unitMgr = std::make_unique<core::UnitManager>();
    core::Player playerOne({ 1 });
    core::Player playerTwo({ 2 });  // Ai module isn't ready yet, so will set just fake player
    builder.m_gameEngine = std::make_unique< core::GameEngine>(boardModel, std::move(unitMgr), playerOne.getId(), playerTwo.getId());
    builder.m_gameEngine.get()->setPlayer(playerOne);
    builder.m_gameEngine.get()->setPlayer(playerTwo);

    auto movementController = controllers::GameController(builder.m_gameEngine.get(), launchSettings.m_host);
    auto unitCreatorController = controllers::UnitSetupContoller(builder.m_gameEngine.get(), launchSettings.m_host);
    builder.m_worldView = std::make_unique< graphics::GameWorldView>(target, boardView, std::move(movementController));
    builder.m_worldView->setUnitSetupController(std::move(unitCreatorController));
    builder.m_gameEngine->subscribe(builder.m_worldView.get());
    builder.m_worldView->addNullUnit(builder.m_tankFactory.createNullUnitView());
    builder.m_worldView->setSetupArea(boardModel.getSetupArea(launchSettings.m_host));
    //builder.m_gameEngine.get()->setUpdateViewInterface(builder.m_worldView.get());

    return builder;
}

GameBuilder& GameBuilder::singleGameMode()
{
    // for alpha version just add one enemy unit to test graphcial things( burning, tooltips, etc...)
    const auto firstUnitPos = core::GameTile(-10, 29);
    const PlayerIdentifier player{ 2 };
    const Angle rotation{ 270.f };
    auto tmpUnit1 = m_tankFactory.createUnitModel(core::UnitType::BasicTank, player);
    tmpUnit1->setPosition(firstUnitPos);
    tmpUnit1->setGunRotation(rotation);

    UnitIdentifier addedUnitID = m_gameEngine->addNewUnit(std::move(tmpUnit1));
    if (!addedUnitID) { return *this; }
    constexpr float scale = 0.17f;
    auto tmpUnitView = m_tankFactory.createUnitView(core::UnitType::BasicTank, addedUnitID, scale);
    tmpUnitView->setRotation(rotation.angle);
    m_worldView->addNewUnitView(std::move(tmpUnitView), firstUnitPos);

    return *this;
}
