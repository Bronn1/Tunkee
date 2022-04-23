#include "units_factory.h"

#include "graphics/tank_view.h"


TanksFactory::TanksFactory()
{
    using enum textures::ID;
    std::string_view T34BodyTextureFilename = "data/textures/TopDown_soldier_tank_body.png";
    std::string_view T34TurretTextureFilename = "data/textures/TopDown_soldier_tank_turrent.png";
    std::string_view explosionTextureFilename = "data/textures/Explosion.png";
    std::string_view mediumMissileFilename =    "data/textures/Medium_Shell.png";
    //std::string_view tooltipTexturesFilename =  "data/textures/tooltip_test.png";

    std::string_view burningIconFilename = "data/textures/icons/burning_icon.png";
    std::string_view loaderIconFilename = "data/textures/icons/loader_icon.png";
    std::string_view crewShellshockedIconFilename = "data/textures/icons/crew_shellshocked_icon.png";
    std::string_view engineDestroyedIconFilename = "data/textures/icons/engine_destroyed_icon.png";
    std::string_view explodedIconFilename = "data/textures/icons/exploded_icon.png";
    std::string_view gunDestroyedIconFilename = "data/textures/icons/gun_destroyed_icon.png";
    std::string_view hiddenIconFilename = "data/textures/icons/hidden_icon.png";
    std::string_view ricochetIconFilename = "data/textures/icons/ricochet_icon.png";
    std::string_view trackDamagedIconFilename = "data/textures/icons/track_damaged_icon.png";

    std::string_view movementInfoIconFilename = "data/textures/icons/movement_icon.png";
    std::string_view shootingInfoIconFilename = "data/textures/icons/shooting_icon.png";

    m_textures.load(T34TankBody, T34BodyTextureFilename);
    m_textures.load(T34TankTurret, T34TurretTextureFilename);
    m_textures.load(Explosion, explosionTextureFilename);
    m_textures.load(Missile, mediumMissileFilename);

    m_textures.load(BurningIcon, burningIconFilename);
    m_textures.load(LoaderKilledIcon, loaderIconFilename);
    m_textures.load(CrewShellshockedIcon, crewShellshockedIconFilename);
    m_textures.load(EngineDestroyedIcon, engineDestroyedIconFilename);
    m_textures.load(ExplodedIcon, explodedIconFilename);
    m_textures.load(GunDestroyedIcon, gunDestroyedIconFilename);
    m_textures.load(HiddenDamageIcon, hiddenIconFilename);
    m_textures.load(MovementInfoIcon, movementInfoIconFilename);
    m_textures.load(ShootingInfoIcon, shootingInfoIconFilename);
   // m_textures.load(Missile, ricochetIconFilename);
   // m_textures.load(Missile, trackDamagedIconFilename);
}


TanksFactory::UnitModelPtr TanksFactory::createBacisTank(const core::GameTile& pos, const Angle rotation, const PlayerIdentifier playerID)
{
    TileDistance moveDistance{ 5 };
    return createTank(pos, rotation, moveDistance, playerID);
}

TanksFactory::UnitViewPtr TanksFactory::createBacisTankView(const UnitIdentifier id, const Angle rotation, const float scale)
{
    return createTankView(id, rotation, scale);
}

TanksFactory::UnitViewPtr TanksFactory::createNullUnitView()
{
    return std::make_unique<graphics::NullUnitView>( m_textures);
}

TanksFactory::UnitModelPtr TanksFactory::createTank(const core::GameTile& pos, const Angle rotation, const TileDistance movement, const PlayerIdentifier playerID)
{
    UnitIdentifier unitId{ 1 };
    HexVertexNumber vertexRotation{ 3 };
    std::unique_ptr<core::Unit> basicTank = std::make_unique<core::TankUnit>(unitId, movement, Shots{ 12 });
    basicTank->setPosition(pos);
    basicTank->setOwner(playerID);
    basicTank->setGunRotation(rotation);
    basicTank->setBodyRotation(vertexRotation);

    return basicTank;
}

TanksFactory::UnitViewPtr TanksFactory::createTankView(const UnitIdentifier id, const Angle rotation, const float scale)
{
    auto basicTankView = std::make_unique<graphics::TankView>(UnitIdentifier{ id }, graphics::TankView::Type::Basic, m_textures);
    basicTankView->setRotation(rotation.angle);
    basicTankView->setScale(scale, scale);

    return basicTankView;
}

