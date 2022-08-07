#pragma once

#include "src/core/i_engine.h"
#include "src/core/data_types.h"
#include "src/core/game_tile.h"
#include "src/units_factory.h"

#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include <set>
#include <optional>

namespace controllers {
    class GameController
    {
    public:
        GameController(core::IEngine* engine, const PlayerIdentifier player);
        void moveUnit(const UnitIdentifier unitID, const core::GameTile& dest) const;
        void onChangeUnitRotation(const UnitIdentifier unitID, const Angle& rotation, const RotateUnitActiom::Type rotationType);
        void onUnitClicked(const UnitIdentifier selectedUnitID, const UnitIdentifier clickedUnitID) const ;
        void onShowUnitStateMsg(const UnitIdentifier selectedUnitID, const UnitIdentifier targetUnitID, const Angle& requiredGunAngleToShot) const;
        void finishSetupStage(PlayerIdentifier playerId);
        void finishActionPhase(PlayerIdentifier playerId);
        bool isEnemyUnitId(const UnitIdentifier unitID) { return !m_ownUnits.contains(unitID); }
        
        void addOwnUnit(const UnitIdentifier unitId) { m_ownUnits.insert(unitId); }
        void addEnemyUnit(const UnitIdentifier unitId) { m_ownUnits.insert(unitId); }
        void deleteOwnUnit(const UnitIdentifier unitId) { m_ownUnits.erase(unitId); }
        void deleteEnemyUnit(const UnitIdentifier unitId) { m_ownUnits.erase(unitId); }
    private:
        core::IEngine* m_gameEngine;
        std::set<UnitIdentifier> m_ownUnits;
        std::set<UnitIdentifier> m_enemyUnits;
        PlayerIdentifier m_player;
    };

    using UnitNodePtr = std::unique_ptr<graphics::IUnitView>;

    class UnitSetupContoller
    {
    public:
        UnitSetupContoller() = default;
        UnitSetupContoller(core::IEngine* engine, const PlayerIdentifier player);
        std::optional<UnitNodePtr> addUnit(const core::GameTile& position);
        //void setWorldView(graphics::GameWorldView* worldView) { m_worldView = worldView; };
    private:
        core::IEngine* m_gameEngine;
        PlayerIdentifier m_player;
        TanksFactory  m_tankFactory;
    };
}

