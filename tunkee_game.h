#pragma once

#include "src/graphics/game_world_view.h"

#include <SFML/Graphics/RenderWindow.hpp>

class TunkeGame 
{
public:
	TunkeGame();
	void        run();

private:
	sf::RenderWindow       m_window;
	//
	//TextureHolder			mTextures;
	//FontHolder				mFonts;

};