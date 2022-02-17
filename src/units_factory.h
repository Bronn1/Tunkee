#pragma once

#include "core/unit.h"
#include "graphics/tank_view.h"
#include "core/data_types.h"
#include "graphics/resource_holder.h"

#include <memory>
#include <SFML/Graphics/Texture.hpp>

using UnitViewPtr = std::unique_ptr < graphics::UnitView>;
using UnitModelPtr = std::unique_ptr<core::Unit>;

/*class UnitFactory
{
protected:
    virtual UnitModelPtr createUnit(UnitIdentifier id) const = 0;
    virtual SceneNodePtr createUnitView(UnitIdentifier id) const = 0;
};*/

class TanksFactory 
{
public:
    TanksFactory();
    UnitModelPtr createBacisTank(const core::GameTile& pos, const Angle rotation, const PlayerIdentifier);
    UnitViewPtr createBacisTankView(const UnitIdentifier id, const Angle rotation, const  float scale);
    UnitViewPtr createNullUnitView();
private:
    UnitModelPtr createTank(const core::GameTile& pos, const Angle rotation, const TileDistance movement, const PlayerIdentifier);
    UnitViewPtr createTankView(const UnitIdentifier id, const Angle rotation, const float scale);
private:
    TextureHolder m_textures;

};