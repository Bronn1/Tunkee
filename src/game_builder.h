#pragma once

#include "core/game_board.h"
#include "graphics/board_view.h"
#include "graphics/game_world_view.h"
#include "units_factory.h"
#include "graphics/resource_holder.h"

#include <memory>
#include <vector>

class GameBuilder
{
public:
    using BoardModelPtr = std::unique_ptr<core::GameBoard>;
    using WorldViewPtr = std::unique_ptr<graphics::GameWorldView>;
    using GameEnginePtr = std::unique_ptr<core::GameEngine>;

    /**
     * @brief Starting point of builder, inits Game model and world view
     * @param target -  SFML window to render graphics
     * @return 
    */
    static GameBuilder  initGameEngine(sf::RenderWindow& target);
    inline WorldViewPtr  getWorldView() { return std::move(m_worldView); }
    /**
    * @brief Adds all stuff for single player game.
    * Should also init game AI (maybe few parameters for AI difficulty).
    * Currently just adds enemy unit for alpha tests(cuz AI module isn't ready yet).
    */
    GameBuilder& singleGameMode();
private:
    friend class graphics::GameWorldView;
    GameBuilder() = default;
    std::vector<core::GameTileType> generateMinimalTileset() const;
    void loadTileTextures();
private:
    core::GameBoard m_boardModel;
    WorldViewPtr m_worldView;
    GameEnginePtr m_gameEngine;
    TanksFactory m_tankFactory;
    TextureHolder m_tileTextures;

};