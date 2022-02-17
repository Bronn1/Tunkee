#pragma once

#include "src/core/game_engine.h"
#include "src/core/data_types.h"
#include "src/core/game_tile.h"
#include "src/graphics/scene_node.h"
#include "src/units_factory.h"

#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include <set>
#include <optional>

namespace controllers {
    class GameController
    {
    public:
        GameController(core::GameEngine* engine, const PlayerIdentifier player);
        void moveUnit(const UnitIdentifier unitID, const core::GameTile& dest) const;
        void onChangeUnitRotation(const UnitIdentifier unitID, const Angle& rotation, const RotateUnitActiom::Type rotationType);
        void onUnitClicked(const UnitIdentifier selectedUnitID, const UnitIdentifier clickedUnitID) const ;
        void finishSetupStage(PlayerIdentifier playerId);
        void finishActionPhase(PlayerIdentifier playerId);
        bool isEnemyUnitId(const UnitIdentifier unitID) { return !m_ownUnits.contains(unitID); }
        
        void addOwnUnit(const UnitIdentifier unitId) { m_ownUnits.insert(unitId); }
        void addEnemyUnit(const UnitIdentifier unitId) { m_ownUnits.insert(unitId); }
        void deleteOwnUnit(const UnitIdentifier unitId) { m_ownUnits.erase(unitId); }
        void deleteEnemyUnit(const UnitIdentifier unitId) { m_ownUnits.erase(unitId); }
    private:
        core::GameEngine* m_gameEngine;
        std::set<UnitIdentifier> m_ownUnits;
        std::set<UnitIdentifier> m_enemyUnits;
        PlayerIdentifier m_player;
    };

    using SceneNodePtr = std::unique_ptr<graphics::SceneNode>;

    class UnitSetupContoller
    {
    public:
        UnitSetupContoller() = default;
        UnitSetupContoller(core::GameEngine* engine, const PlayerIdentifier player);
        std::optional<UnitViewPtr> addUnit(const core::GameTile& position);
        //void setWorldView(graphics::GameWorldView* worldView) { m_worldView = worldView; };
    private:
        core::GameEngine* m_gameEngine;
        PlayerIdentifier m_player;
        TanksFactory  m_tankFactory;
    };
}

