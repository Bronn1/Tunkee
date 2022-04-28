#pragma once

#include "text_label.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace sf {
    class Event;
}

namespace gui {
class MainMenu : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
    enum class Dialogs
    {
        QuitGame,
        SinglePlayer,
        None
    };
    MainMenu(const float windowWidth);
    void  draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void initAlphaVersionTitle_v_0_0_2(const float windowWidth);
    Dialogs  update(const sf::Event& event);
private:
    sf::Font m_labelsFont;
    TextLabel m_version;
    TextLabel m_alphaVersionReminder;
};

}

