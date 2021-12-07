#include "units_factory.h"

#include "graphics/tank_view.h"


TanksFactory::TanksFactory()
{
    const std::string kT34BodyTexture = "data/textures/TopDown_soldier_tank_body.png";
    const std::string kT34BodyTurrent = "data/textures/TopDown_soldier_tank_turrent.png";
    m_textures.load(textures::ID::T34TankBody, kT34BodyTexture);
    m_textures.load(textures::ID::T34TankTurret, kT34BodyTurrent);
}

UnitModelPtr TanksFactory::createBacisTank(const core::GameTile& pos, const  const PlayerIdentifier playerID)
{
    TileDistance moveDistance{ 7 };
    return createTank(pos, moveDistance, playerID);
}

SceneNodePtr TanksFactory::createBacisTankView(const UnitIdentifier id, const float rotation, const float scale)
{
    return createTankView(id, rotation, scale);
}

UnitModelPtr TanksFactory::createTank(const core::GameTile& pos, const TileDistance moveDistance, const PlayerIdentifier playerID)
{
    UnitIdentifier unitId{ 1 };
    std::unique_ptr<core::Unit> basicTank = std::make_unique<core::BasicTank>(unitId, moveDistance);
    (*basicTank).setPosition(pos);
    (*basicTank).setOwner(playerID);
   // auto tank = std::make_unique<
    return basicTank;
}

SceneNodePtr TanksFactory::createTankView(const UnitIdentifier id, const float rotation, const float scale)
{
    auto basicTankView = std::make_unique<graphics::TankView>(UnitIdentifier{ id }, graphics::TankView::Type::Basic, m_textures.get(textures::ID::T34TankBody), m_textures.get(textures::ID::T34TankTurret));
    (*basicTankView).setRotation(rotation);
    (*basicTankView).setScale(scale, scale);

    return basicTankView;
}

