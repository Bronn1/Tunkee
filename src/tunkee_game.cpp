#include "tunkee_game.h"
#include "graphics/board_view.h"
#include "core/game_board.h"
#include "core/game_engine.h"
#include "core/unit_manager.h"
#include "graphics/tank_view.h"
#include "units_factory.h"
#include "game_builder.h"

#include <chrono>
#include <thread>

//const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
constexpr int kResHeight = 1800;
constexpr int kResWidth = 1000;

TunkeGame::TunkeGame() :
    m_window(sf::VideoMode(kResHeight, kResWidth), "Tunkee", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize, sf::ContextSettings(0, 0, 8))
{
}

void TunkeGame::runGameLoop()
{
    // TODO  add texture holder
    sf::Texture      m_grassTexture1;
    if (!m_grassTexture1.loadFromFile("data/textures/grass-test1.jpg"))
        std::cerr << "Can not load texture\n";
    m_grassTexture1.setSmooth(true);

    auto worldBuilder = GameBuilder::createMinimalGame(m_window, &m_grassTexture1);
    worldBuilder.addAi();
    auto testBoard = worldBuilder.getBoardModel();
    auto worldView = worldBuilder.getWorldView();
    sf::Color backgroundGrass{ 83, 132, 66 };
    sf::Event event;
    while (m_window.isOpen()) {
        m_window.clear(backgroundGrass);
        worldView->draw();
        worldView->update(event);
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
