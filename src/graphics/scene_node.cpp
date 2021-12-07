#include "scene_node.h"

#include <algorithm>
#include <cassert>



graphics::SceneNode::SceneNode()
	: m_children()
	, m_parent(nullptr)
{
}

void graphics::SceneNode::attachChild(SceneNodePtr child)
{
	child->m_parent = this;
	m_children.push_back(std::move(child));
}

graphics::SceneNode::SceneNodePtr graphics::SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(m_children.begin(), m_children.end(), [&](SceneNodePtr& p) { return p.get() == &node; });
	assert(found != m_children.end());

	SceneNodePtr result = std::move(*found);
	result->m_parent = nullptr;
	m_children.erase(found);
	return result;
}

void graphics::SceneNode::update(sf::Time dt)
{
	updateCurrent(dt);
	updateChildren(dt);
}

void graphics::SceneNode::updateCurrent(sf::Time)
{
	// Do nothing by default
}

void graphics::SceneNode::updateChildren(sf::Time dt)
{
	for (const auto& child : m_children)
	{
		child->update(dt);
	}
}

void graphics::SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Apply transform of current node
	states.transform *= getTransform();

	// Draw node and children with changed transform
	drawCurrent(target, states);
	drawChildren(target, states);
}

void graphics::SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
	// Do nothing by default
}

void graphics::SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& child : m_children)
	{
		child->draw(target, states);
	}
}

sf::Vector2f graphics::SceneNode::interpolateWithFactor(const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, float factor) const
{
	if (factor > 1.f)
		factor = 1.f;

	else if (factor < 0.f)
		factor = 0.f;

	return startPoint + (endPoint - startPoint) * factor;
}

sf::Vector2f graphics::SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform graphics::SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->m_parent)
		transform = node->getTransform() * transform;

	return transform;
}


