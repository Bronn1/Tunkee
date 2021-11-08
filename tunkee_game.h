#pragma once

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