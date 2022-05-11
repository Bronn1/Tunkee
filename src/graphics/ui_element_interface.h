#pragma once

#include "scene_node.h"

namespace sf {
class Event;
}


namespace graphics
{
/**
 * @brief abstract class for user interface in game like buttons, clickable layouts, tables, etc..
*/
class UIElement : public ISceneNode {
public:
    virtual void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) = 0;
    virtual sf::Vector2f getSize() const = 0;
    void setVisibility(bool isVisible) { m_isVisible = isVisible; }
    virtual ~UIElement() = default;
protected:
    bool m_isVisible{ true };
};

} // namespace graphics