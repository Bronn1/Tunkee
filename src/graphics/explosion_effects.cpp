#include "explosion_effects.h"

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <cmath>

graphics::PostEffect::~PostEffect()
{
}

void graphics::PostEffect::applyShader(const sf::Shader& shader, sf::RenderTarget& output)
{
    sf::Vector2f outputSize = static_cast<sf::Vector2f>(output.getSize());
    const int vertexCount = 4;
    sf::VertexArray vertices(sf::TrianglesStrip, vertexCount);
    vertices[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Vector2f(0, 1));
    vertices[1] = sf::Vertex(sf::Vector2f(outputSize.x, 0), sf::Vector2f(1, 1));
    vertices[2] = sf::Vertex(sf::Vector2f(0, outputSize.y), sf::Vector2f(0, 0));
    vertices[3] = sf::Vertex(sf::Vector2f(outputSize), sf::Vector2f(1, 0));

    sf::RenderStates states;
    states.shader = &shader;
    states.blendMode = sf::BlendNone;

    output.draw(vertices, states);
}

bool graphics::PostEffect::isSupported()
{
    return sf::Shader::isAvailable();
}

graphics::Animation::Animation() :
    m_sprite(), m_frameSize(),
    m_numFrames(0), m_currentFrame(0),
    m_duration(sf::Time::Zero),
    m_elapsedTime(sf::Time::Zero), m_repeat(false)
{
}

graphics::Animation::Animation(const sf::Texture& texture):
    m_sprite(texture), m_frameSize(),
    m_numFrames(0), m_currentFrame(0), 
    m_duration(sf::Time::Zero), 
    m_elapsedTime(sf::Time::Zero) , m_repeat(false)
{
    sf::IntRect textureRect = sf::IntRect(0, 0, m_frameSize.x, m_frameSize.y);
    m_sprite.setTextureRect(textureRect);
}

void graphics::Animation::centerOrigin()
{
    sf::FloatRect bounds = getLocalBounds();
    setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void graphics::Animation::update(sf::Time dt)
{
    if (!m_isActive || isFinished()) return;

    sf::Time timePerFrame = m_duration / static_cast<float>(m_numFrames);
    m_elapsedTime += dt;

    sf::Vector2i textureBounds(m_sprite.getTexture()->getSize());
    sf::IntRect textureRect = m_sprite.getTextureRect();

    if (m_currentFrame == 0)
        textureRect = sf::IntRect(0, 0, m_frameSize.x, m_frameSize.y);

    // While we have a frame to process
    while (m_elapsedTime >= timePerFrame && (m_currentFrame <= m_numFrames || m_repeat))
    {
        // Move the texture rect left
        textureRect.left += textureRect.width;

        // If we reach the end of the texture
        if (textureRect.left + textureRect.width > textureBounds.x)
        {
            // Move it down one line
            textureRect.left = 0;
            textureRect.top += textureRect.height;
        }

        // And progress to next frame
        m_elapsedTime -= timePerFrame;
        if (m_repeat)
        {
            m_currentFrame = (m_currentFrame + 1) % m_numFrames;

            if (m_currentFrame == 0)
                textureRect = sf::IntRect(0, 0, m_frameSize.x, m_frameSize.y);
        }
        else
        {
            m_currentFrame++;
        }
    }

    m_sprite.setTextureRect(textureRect);
}

void graphics::Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_isActive || isFinished()) return;

    states.transform *= getTransform();
    target.draw(m_sprite, states);
}
