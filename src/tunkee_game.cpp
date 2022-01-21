#include "tunkee_game.h"
#include "gui/gui.h"
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

void TunkeGame::run()
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

    while (m_window.isOpen()) {
        sf::Event event;
        m_window.clear(sf::Color::Green);
        worldView->draw();
        worldView->update(event);
        // should use screen stack
        /*while (m_window.pollEvent(event)) {
            worldView.update(event.type);

            if (event.type == sf::Event::Closed)
                m_window.close();
        }*/

        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        m_window.display();
    }
}
