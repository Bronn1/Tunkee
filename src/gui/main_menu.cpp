#include "main_menu.h"
#include "text_label.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <string_view>

constexpr int kSmallLabelCharacterSize = 15;
constexpr int kLargelLabelCharacterSize = 40;
const sf::Color kSemiTransparentGrey = sf::Color(192, 192, 192, 100);
constexpr std::string_view kVersion = "Alpha build v0.0.2";
const sf::Time kTimePerFrame = sf::milliseconds(9);

gui::MainMenu::MainMenu(const float windowWidth) :
    m_alphaVersionReminder(m_labelsFont, kLargelLabelCharacterSize, sf::Color::White),
    m_version(m_labelsFont, kSmallLabelCharacterSize, kSemiTransparentGrey)
{
    // TODO move all paths and fonts to texture manager
    m_labelsFont.loadFromFile("data/examples/comic.ttf");
    initAlphaVersionTitle_v_0_0_2(windowWidth);
    
}

void gui::MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_version, states);
    target.draw(m_alphaVersionReminder, states);
}

void gui::MainMenu::initAlphaVersionTitle_v_0_0_2(const float windowWidth)
{
    const std::string test(kVersion);
    m_version.setText(test);
    m_version.setPosition({ windowWidth - m_version.getWidth() - 20, 0 });
    m_version.setColor(kSemiTransparentGrey);
    const std::string buildInfo("Alpha build. To start the game press any key...");
    m_alphaVersionReminder.setText(buildInfo);
    m_alphaVersionReminder.setPosition({ ( windowWidth  - m_alphaVersionReminder.getWidth()) / 2.f  ,  windowWidth / 4.f });
    m_alphaVersionReminder.setColor(kSemiTransparentGrey);
}

gui::MainMenu::Dialogs gui::MainMenu::update(const sf::Event& event)
{
    using enum gui::MainMenu::Dialogs;
    Dialogs dialogStatus = None;
    switch (event.type)
    {
    case sf::Event::KeyPressed:
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            dialogStatus = SinglePlayer;
        }else {
            dialogStatus = QuitGame;
        }
        break;
    case sf::Event::Closed:
        dialogStatus = QuitGame;
        break;
    default:
        break;
    }
    return dialogStatus;
}
