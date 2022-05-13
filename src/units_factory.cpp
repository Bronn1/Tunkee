#include "units_factory.h"
#include "graphics/tank_view.h"

 
TanksFactory::UnitModelPtr TanksFactory::createUnitModel(const core::UnitType type, const PlayerIdentifier id) const
{
    switch (type) {
    case core::UnitType::BasicTank:
        return createBasicTank(id);
    default:
        return createBasicTank(id);
        break;
    }
}

TanksFactory::UnitViewPtr TanksFactory::createUnitView(const core::UnitType type, const UnitIdentifier id, const float scale) const
{
    switch (type) {
    case core::UnitType::BasicTank:
        return createBasicTankView(id, scale);
    default:
        return createBasicTankView(id, scale);
        break;
    }
}

TanksFactory::UnitModelPtr TanksFactory::createBasicTank(const PlayerIdentifier playerId) const
{
    UnitIdentifier unitId{ 1 };
    HexVertexNumber vertexRotation{ 3 };
    std::unique_ptr<core::Unit> basicTank = std::make_unique<core::TankUnit>(unitId, TileDistance{ 5 }, Shots{ 12 });
    //basicTank->setPosition(pos);
    basicTank->setOwner(playerId);
    //basicTank->setGunRotation(rotation);
    basicTank->setBodyRotation(vertexRotation);

    return basicTank;
}

TanksFactory::UnitViewPtr TanksFactory::createBasicTankView(const UnitIdentifier id, const float scale) const
{
    auto basicTankView = std::make_unique<graphics::TankView>(UnitIdentifier{ id }, graphics::TankView::Type::Basic, getTexureHolder());
   // basicTankView->setRotation(rotation.angle);
    basicTankView->setScale(scale, scale);

    return basicTankView;
}

UnitFactory::UnitFactory()
{
    loadUnitTextures();
}

UnitFactory::UnitViewPtr UnitFactory::createNullUnitView()
{
    return std::make_unique<graphics::NullUnitView>(m_textures);
}

void UnitFactory::loadUnitTextures()
{
    using enum textures::ID;
    std::string_view T34BodyTextureFilename = "data/textures/TopDown_soldier_tank_body.png";
    std::string_view T34TurretTextureFilename = "data/textures/TopDown_soldier_tank_turrent.png";
    std::string_view T34BodyDestroyedTextureFilename = "data/textures/destroyedBasicTank2.png";
    std::string_view T34TurretDestroyedTextureFilename = "data/textures/destroyedBasicTurret.png";
    std::string_view explosionTextureFilename = "data/textures/Explosion.png";
    std::string_view mediumMissileFilename = "data/textures/Medium_Shell.png";
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
    m_textures.load(T34TankBodyDestroyed, T34BodyDestroyedTextureFilename);
    m_textures.load(T34TankTurretDestroyed, T34TurretDestroyedTextureFilename);
    m_textures.load(Explosion, explosionTextureFilename);
    m_textures.load(Missile, mediumMissileFilename);
    // icons for unit tooltips
    m_textures.load(BurningIcon, burningIconFilename);
    m_textures.load(LoaderKilledIcon, loaderIconFilename);
    m_textures.load(CrewShellshockedIcon, crewShellshockedIconFilename);
    m_textures.load(EngineDestroyedIcon, engineDestroyedIconFilename);
    m_textures.load(ExplodedIcon, explodedIconFilename);
    m_textures.load(GunDestroyedIcon, gunDestroyedIconFilename);
    m_textures.load(HiddenDamageIcon, hiddenIconFilename);
    m_textures.load(MovementInfoIcon, movementInfoIconFilename);
    m_textures.load(ShootingInfoIcon, shootingInfoIconFilename);
}
