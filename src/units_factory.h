#pragma once

#include "core/unit.h"
#include "graphics/tank_view.h"
#include "core/data_types.h"
#include "graphics/resource_holder.h"

#include <memory>
#include <SFML/Graphics/Texture.hpp>


class UnitFactory 
{
public:
    using UnitViewPtr = std::unique_ptr<graphics::IUnitView>;
    using UnitModelPtr = std::unique_ptr<core::Unit>;

    virtual UnitModelPtr createUnitModel(const core::UnitType type, const PlayerIdentifier) const = 0;
    virtual UnitViewPtr createUnitView(const core::UnitType type, const UnitIdentifier id, const  float scale) const = 0;
    UnitViewPtr createNullUnitView();
    const TextureHolder& getTexureHolder() const { return m_textures; }
    void loadUnitTextures();
private:
    TextureHolder m_textures;
};

class TanksFactory : public UnitFactory
{
public:
    TanksFactory();
    UnitModelPtr  createUnitModel(const core::UnitType type, const PlayerIdentifier) const override;
    UnitViewPtr  createUnitView(const core::UnitType type, const UnitIdentifier id, const  float scale) const override;
private:
    UnitModelPtr createBasicTank( const PlayerIdentifier id) const;
    UnitViewPtr createBasicTankView(const UnitIdentifier id,  const float scale) const;
};