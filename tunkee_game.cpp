#include "tunkee_game.h"
#include "src/gui/gui.h"
#include "src/graphics/board_view.h"
#include "src/core/game_board.h"
#include "src/core/game_rules_engine.h"
#include "src/core/unit_manager.h"
#include "src/graphics/tank_view.h"

#include "src/builders/game_builder.h"

#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>


using UnitManagerPtr = std::shared_ptr<core::UnitManager>;
const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

TunkeGame::TunkeGame() :
	m_window(sf::VideoMode(1800, 1000), "Tunkee", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize, sf::ContextSettings(0, 0, 8))
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
    auto testBoard = worldBuilder.getBoardModel();
    auto worldView = worldBuilder.getWorldView();

    // ############### Code to test first version
    //auto unitMgr = std::make_shared<core::UnitManager>();
    sf::Texture m_grassTexture;
    if (!m_grassTexture.loadFromFile("data/textures/TopDown_soldier_tank_body.png"))
        std::cerr << "Can not load texture\n";
    m_grassTexture.setSmooth(true);

    sf::Texture m_grassTexture2;
    if (!m_grassTexture2.loadFromFile("data/textures/TopDown_soldier_tank_turrent.png"))
        std::cerr << "Can not load texture\n";
    m_grassTexture2.setSmooth(true);

    std::unique_ptr< graphics::TankView > air = std::make_unique<graphics::TankView>(ObjectIdentifier{ 1 }, graphics::TankView::Type::Eagle, m_grassTexture, m_grassTexture2);
    (*air).setPosition({ 289.827f ,326.928f });
    (*air).setRotation(270);
    //air.setOrigin(100, 20);
    (*air).setScale(0.17f, 0.17f);
    bool wasButtonPresssed = false;

    std::unique_ptr< graphics::TankView > air2 = std::make_unique<graphics::TankView>(ObjectIdentifier{ 2 }, graphics::TankView::Type::Eagle, m_grassTexture, m_grassTexture2);
    (*air2).setPosition({ 250.f ,349.928f });
    (*air2).setRotation(270);
    (*air2).setScale(0.17f, 0.17f);

     (*worldView).addNewUnitView(std::move(air), core::GameTile(0, 0));
    // (*worldView).addNewUnitView(std::move(air2), core::GameTile(0, 0));
     // ############### 

    while (m_window.isOpen()) {
        sf::Event event;
        m_window.clear(sf::Color::Green);
        (*worldView).draw();
        (*worldView).update(event);
        // should use screen stack
        /*while (m_window.pollEvent(event)) {
            worldView.update(event.type);

            if (event.type == sf::Event::Closed)
                m_window.close();
        }*/

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        m_window.display();
    }
}