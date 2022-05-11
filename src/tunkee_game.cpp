#include "tunkee_game.h"
#include "graphics/board_view.h"
#include "core/game_board.h"
#include "core/game_engine.h"
#include "core/unit_manager.h"
#include "graphics/tank_view.h"
#include "units_factory.h"
#include "game_builder.h"
#include "common.h"

#include <chrono>
#include <thread>

constexpr int kResHeight = 900;
constexpr int kResWidth = 1600;
const sf::Time kTimePerFrame = sf::milliseconds(9);
constexpr  std::string_view kGameTitle = "Tunkee";

TunkeGame::TunkeGame() :
    m_window(sf::VideoMode(kResWidth, kResHeight), std::string(kGameTitle), sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize, sf::ContextSettings(0, 0, 8))
{
}

void TunkeGame::runGameLoop()
{
    auto worldBuilder = GameBuilder::initGameEngine(m_window);
    worldBuilder.singleGameMode();
    //auto testBoard = worldBuilder.getBoardModel();
    auto worldView = worldBuilder.getWorldView();
    sf::Color backgroundGrass{ 83, 132, 66 };
    sf::Event event;
    while (m_window.isOpen()) {
        m_window.clear(backgroundGrass);
        worldView->draw();
        if (worldView->handleEvent(event) == common::kExitGameSession){
            return;
        }
        worldView->update(kTimePerFrame);
        m_window.display();
    }
}

void TunkeGame::run()
{
    sf::Color backgroundGrass{ 83, 132, 66 };
    sf::Event event;
    gui::MainMenu menu(static_cast<float>(m_window.getSize().x));
    using enum gui::MainMenu::Dialogs;
    while (m_window.isOpen()) {
        m_window.clear(backgroundGrass);
        m_window.draw(menu);
        while (m_window.pollEvent(event)) {
            gui::MainMenu::Dialogs dialogStatus =  menu.update(event);
            switch (dialogStatus)
            {
            case SinglePlayer:
                runGameLoop();
                break;
            case QuitGame:
                return;
            default:
                break;
            }
        }
        m_window.display();
    }
}
