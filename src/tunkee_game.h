#pragma once

#include "graphics/game_world_view.h"
#include "gui/main_menu.h"
#include <SFML/Graphics/RenderWindow.hpp>

class TunkeGame 
{
public:
    TunkeGame();
    void runGameLoop();
    void run();
    //void handleDialogStatus(const gui::MainMenu::Dialogs dialogStatus);

private:
    sf::RenderWindow m_window;
};