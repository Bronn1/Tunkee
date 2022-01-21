#include "game_tile.h"

std::ostream& core::operator<<(std::ostream& os, const GameTile& gt)
{
	os << "[" << gt.m_q << ", " << gt.m_r << ", " << gt.m_s << "]";
	return os;
}

core::GameTile core::NullTile()
{
	return GameTile(-9999, -9999);
}
