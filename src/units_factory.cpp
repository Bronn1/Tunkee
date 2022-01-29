#include "units_factory.h"

#include "graphics/tank_view.h"


TanksFactory::TanksFactory()
{
    using enum textures::ID;
    std::string_view T34BodyTextureFilename = "data/textures/TopDown_soldier_tank_body.png";
    std::string_view T34TurretTextureFilename = "data/textures/TopDown_soldier_tank_turrent.png";
    std::string_view explosionTextureFilename = "data/textures/Explosion.png";
    std::string_view mediumMissileFilename =    "data/textures/Medium_Shell.png";
    m_textures.load(T34TankBody, T34BodyTextureFilename);
    m_textures.load(T34TankTurret, T34TurretTextureFilename);
    m_textures.load(Explosion, explosionTextureFilename);
    m_textures.load(Missile, mediumMissileFilename);
}

UnitModelPtr TanksFactory::createBacisTank(const core::GameTile& pos, const Angle rotation, const PlayerIdentifier playerID)
{
    TileDistance moveDistance{ 5 };
    return createTank(pos, rotation, moveDistance, playerID);
}

UnitViewPtr TanksFactory::createBacisTankView(const UnitIdentifier id, const Angle rotation, const float scale)
{
    return createTankView(id, rotation, scale);
}

UnitViewPtr TanksFactory::createNullUnitView()
{
    return std::make_unique<graphics::NullUnitView>( m_textures);
}

UnitModelPtr TanksFactory::createTank(const core::GameTile& pos, const Angle rotation, const TileDistance movement, const PlayerIdentifier playerID)
{
    UnitIdentifier unitId{ 1 };
    HexVertexNumber vertexRotation{ 3 };
    std::unique_ptr<core::Unit> basicTank = std::make_unique<core::TankUnit>(unitId, movement, Shots{ 12 });
    basicTank->setPosition(pos);
    basicTank->setOwner(playerID);
    basicTank->setGunRotation(rotation);
    basicTank->setUnitVertexRotation(vertexRotation);

    return basicTank;
}

UnitViewPtr TanksFactory::createTankView(const UnitIdentifier id, const Angle rotation, const float scale)
{
    auto basicTankView = std::make_unique<graphics::UnitView>(UnitIdentifier{ id }, graphics::UnitView::Type::Basic, m_textures);
    basicTankView->setRotation(rotation.angle);
    basicTankView->setScale(scale, scale);

    return basicTankView;
}

