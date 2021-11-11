#pragma once

#include "../core/game_board.h"
#include "../graphics/hexagonal_board.h"

#include <memory>
#include <vector>

using BoardModelPtr = std::unique_ptr<core::GameBoard>;
using BoardViewPtr = std::unique_ptr<graphics::GameBoardView>;

class BoardBuilder
{
public:
	BoardBuilder();
	void                                createMinimalBoard();
	//inline BoardModelPtr              getBoardModel() { return std::move(m_boardModel); }
	inline core::GameBoard              getBoardModel() { return std::move(m_boardModel); }
	inline BoardViewPtr                 getBoardView() { return std::move(m_boardView); }


private:
	std::vector<core::GameTileType>     generateMinimalTileset() { return std::vector<core::GameTileType>(25); }
	void                    reset();
	//BoardModelPtr m_boardModel;
	// TODO only for TESTS switch to pointer back
	core::GameBoard m_boardModel;
	BoardViewPtr m_boardView;
};