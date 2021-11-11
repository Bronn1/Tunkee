#include "board_builder.h"

BoardBuilder::BoardBuilder()
{
}

void BoardBuilder::reset()
{
	//m_boardModel.reset();
	m_boardView.reset();
}

void BoardBuilder::createMinimalBoard()
{
	reset();
	std::vector<core::GameTileType> tileset = generateMinimalTileset();
	int boardSize = std::size(tileset);
	// TODO use unique_ptr too, now just for tests
	m_boardModel = core::GameBoard(tileset, boardSize, boardSize);
	m_boardView = std::make_unique<graphics::GameBoardView>(m_boardModel.getTileCoordinates(), boardSize);
	//core::GameBoard testBoard(tileset, 40, 40);
}
