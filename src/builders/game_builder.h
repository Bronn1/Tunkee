#pragma once

#include "../core/game_board.h"
#include "../graphics/board_view.h"
#include "../graphics/game_world_view.h"
#include "../core/unit.h"

#include <memory>
#include <vector>

using BoardModelPtr = std::unique_ptr<core::GameBoard>;
using WorldViewPtr = std::unique_ptr<graphics::GameWorldView>;
using GameEnginePtr = std::unique_ptr<core::GameRulesEngine>;
constexpr int kTestMapSize = 10;

class GameBuilder
{
public:
	
	static GameBuilder  createMinimalGame(sf::RenderWindow& target, sf::Texture* texture);
	//inline BoardModelPtr getBoardModel() { return std::move(m_boardModel); }
	inline core::GameBoard  getBoardModel()   { return std::move(m_boardModel); }
	inline WorldViewPtr  getWorldView()  { return std::move(m_worldView); }
	inline GameEnginePtr  getGameEngine() { return std::move(m_gameEngineRules); }


private:
	GameBuilder();
	std::vector<core::GameTileType> generateMinimalTileset() { return std::vector<core::GameTileType>(kTestMapSize); }
	void reset();
	// TODO only for TESTS switch to pointer back BoardModelPtr m_boardModel;
	core::GameBoard m_boardModel;
	WorldViewPtr m_worldView;
	GameEnginePtr m_gameEngineRules;
};