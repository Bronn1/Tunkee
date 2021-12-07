#pragma once

#include "core/unit.h"
#include "graphics/scene_node.h"
#include "core/data_types.h"
#include "graphics/resource_holder.h"

#include <memory>
#include <SFML/Graphics/Texture.hpp>

using SceneNodePtr = std::unique_ptr<graphics::SceneNode>;
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
	UnitModelPtr createBacisTank(const core::GameTile& pos, const PlayerIdentifier);
	SceneNodePtr createBacisTankView(const UnitIdentifier id, const float rotation, const  float scale);
private:
	UnitModelPtr createTank(const core::GameTile& pos, const TileDistance moveDistance, const PlayerIdentifier);
	SceneNodePtr createTankView(const UnitIdentifier id, const float rotation, const float scale);
private:
	TextureHolder m_textures;

};