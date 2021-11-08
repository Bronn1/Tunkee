#include "tunkee_game.h"
#include "src/gui/gui.h"
#include "src/graphics/hexagonal_board.h"
#include "src/core/game_board.h"
#include "src/core/main_engine.h"
#include "src/core/unit_manager.h"
#include "src/graphics/tank_view.h"

#include "src/board_builder.h"

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
    BoardBuilder boardBuilder{};
    boardBuilder.createMinimalBoard();
    auto testBoard = boardBuilder.getBoardModel();
    auto hexBoard = boardBuilder.getBoardView();

    // for now just like this, later better to use mainEngine Builderr and create unit Mng inside class with created units prototypes
    auto unitMgr = std::make_shared<core::UnitManager>();
    core::MainEngine gameEngine(testBoard, std::move(unitMgr), PlayerIdentifier{ 1 }, PlayerIdentifier{ 2 });

    sf::Texture m_grassTexture;
    if (!m_grassTexture.loadFromFile("data/textures/TopDown_soldier_tank_body.png"))
        std::cerr << "Can not load texture\n";
    m_grassTexture.setSmooth(true);

    sf::Texture m_grassTexture2;
    if (!m_grassTexture2.loadFromFile("data/textures/TopDown_soldier_tank_turrent.png"))
        std::cerr << "Can not load texture\n";
    m_grassTexture2.setSmooth(true);
    graphics::TankView air(graphics::TankView::Type::Eagle, m_grassTexture, m_grassTexture2);
    air.setPosition({ 289.827 ,326.928 });
    air.setRotation(270);
    //air.setOrigin(100, 20);
    air.setScale(0.17, 0.17);
    //air.outl
    bool wasButtonPresssed = false;

    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                m_window.close();
        }

        m_window.clear(sf::Color::Green);
        (*hexBoard).draw(m_window);
        m_window.draw(air);
        // TODO FOR tests only
        auto mouse_pos = sf::Mouse::getPosition(m_window); // Mouse position relative to the window
        auto translated_pos = m_window.mapPixelToCoords(mouse_pos); // Mouse position translated into world coordinates
        air.showTooltip(translated_pos);
        if (air.getBoundingRect().contains(translated_pos)) // Rectangle-contains-point check
        {
            auto [posx, posy] = air.getPosition();
            //air.setScale(0.2, 0.2);
            // m_tileSelector.setPosition(posx, posy);
             //std::cout << posx << " " << posy << "\n";
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            wasButtonPresssed = true;
            sf::Vector2f curPos = air.getPosition();

            // now we have both the sprite position and the cursor
            // position lets do the calculation so our sprite will
            // face the position of the mouse
            const float PI = 3.14159265;

            float dx = curPos.x - mouse_pos.x;
            float dy = curPos.y - mouse_pos.y;

            float turretTextureOffset = 90;
            float rotation = ((atan2(dy, dx)) * 180 / PI) + turretTextureOffset;
            air.moveTo(TimePerFrame);
            air.rotateTurretTo(rotation);
        }
        if (wasButtonPresssed && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            //m_window.create(sf::VideoMode(300,300), "Tunkee", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize, sf::ContextSettings(0, 0, 8));
            if (air.getBoundingRect().contains(translated_pos)) // Rectangle-contains-point check
            {
                air.setAsSelected();
                
            }
            wasButtonPresssed = false;
            air.setPosition(439.91, 275);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        m_window.display();
    }

}
