#include "scene_node.h"

#include <algorithm>
#include <cassert>

void graphics::ISceneNode::attachChild(SceneNodePtr child)
{
    child->m_parent = this;
    m_children.push_back(std::move(child));
}

graphics::ISceneNode::SceneNodePtr graphics::ISceneNode::detachChild(const ISceneNode& node)
{
    auto found = std::find_if(m_children.begin(), m_children.end(), [&](SceneNodePtr& p) { return p.get() == &node; });
    assert(found != m_children.end());

    SceneNodePtr result = std::move(*found);
    result->m_parent = nullptr;
    m_children.erase(found);
    return result;
}

void graphics::ISceneNode::update(sf::Time dt)
{
    updateCurrent(dt);
    updateChildren(dt);
}

void graphics::ISceneNode::updateChildren(sf::Time dt)
{
    for (const auto& child : m_children)
    {
        child->update(dt);
    }
}

void graphics::ISceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Apply transform of current node
    states.transform *= getTransform();

    // Draw node and children with changed transform
    drawCurrent(target, states);
    drawChildren(target, states);
}

void graphics::ISceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& child : m_children)
    {
        child->draw(target, states);
    }
}

sf::Vector2f graphics::ISceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform graphics::ISceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;

    for (const ISceneNode* node = this; node != nullptr; node = node->m_parent)
        transform = node->getTransform() * transform;

    return transform;
}


